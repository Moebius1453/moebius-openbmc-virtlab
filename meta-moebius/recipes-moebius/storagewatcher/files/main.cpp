#include "space_status.hpp"

#include <sdbusplus/bus.hpp>

#include <chrono>
#include <cstdint>
#include <exception>
#include <iostream>
#include <optional>
#include <string>
#include <thread>
#include <variant>

namespace
{
constexpr const char* service = "xyz.moebius.StorageInfo";
constexpr const char* totalPath =
    "/xyz/openbmc_project/metric/bmc/storage/rw/total";
constexpr const char* availablePath =
    "/xyz/openbmc_project/metric/bmc/storage/rw/available";
using storagewatcher::SpaceStatus;
using Clock = std::chrono::steady_clock;
constexpr auto period = std::chrono::seconds(10);

// 普通 D-Bus 消费者：不申请服务名，不发布对象或接口。
double readValue(sdbusplus::bus_t& bus, const char* path)
{
    auto request = bus.new_method_call(service, path,
                                       "org.freedesktop.DBus.Properties", "Get");
    request.append("xyz.openbmc_project.Metric.Value", "Value");
    auto reply = bus.call(request, uint64_t{2000000}); // 每次调用最多等待约 2 秒。
    std::variant<double> value;
    reply.read(value);
    return std::get<double>(value);
}

const char* statusName(SpaceStatus status)
{
    switch (status)
    {
        case SpaceStatus::Critical:
            return "Critical";
        case SpaceStatus::Warning:
            return "Warning";
        case SpaceStatus::Ok:
            return "OK";
        default:
            return "UNKNOWN";
    }
}

// 首次和状态转换才记录；连续失败保持 UNKNOWN，不每轮刷日志。
void report(std::optional<SpaceStatus>& previous, SpaceStatus current,
            const std::string& detail)
{
    if (!previous || *previous != current)
    {
        std::cout << "storagewatcher: source=" << service
                  << " state=" << statusName(current);
        if (previous && *previous == SpaceStatus::Unknown &&
            current != SpaceStatus::Unknown)
        {
            std::cout << " recovered";
        }
        std::cout << " " << detail << std::endl;
        previous = current;
    }
}
} // namespace

int main()
{
    std::optional<sdbusplus::bus_t> bus;
    std::optional<SpaceStatus> previous;
    auto nextSample = Clock::now();

    while (true)
    {
        try
        {
            if (!bus)
            {
                bus.emplace(sdbusplus::bus::new_system());
            }
            const double total = readValue(*bus, totalPath);
            const double available = readValue(*bus, availablePath);
            const auto status = storagewatcher::classify(total, available);
            std::string detail = "total=" + std::to_string(total) +
                                 " available=" + std::to_string(available);
            if (status == SpaceStatus::Unknown)
            {
                detail += " invalid metric values";
            }
            else
            {
                detail += " available_percent=" +
                          std::to_string((available / total) * 100.0);
            }
            report(previous, status, detail);
        }
        catch (const std::exception& error)
        {
            report(previous, SpaceStatus::Unknown, error.what());
            bus.reset(); // 下轮重新连接，也能等待发布者稍后启动。
        }

        // 固定 steady_clock 时间网格；调用超时后跳过错过的周期，不追赶连读。
        const auto now = Clock::now();
        nextSample += period;
        if (nextSample <= now)
        {
            nextSample += period * ((now - nextSample) / period + 1);
        }
        try
        {
            while (bus && Clock::now() < nextSample)
            {
                bus->process_discard();
                const auto remaining = nextSample - Clock::now();
                if (remaining <= Clock::duration::zero())
                {
                    break;
                }
                bus->wait(std::chrono::ceil<std::chrono::microseconds>(remaining)
                              .count());
            }
        }
        catch (const std::exception& error)
        {
            report(previous, SpaceStatus::Unknown, error.what());
            bus.reset();
        }
        // 总线不可用时仍休眠到下一周期，避免异常重试忙转。
        if (!bus)
        {
            std::this_thread::sleep_until(nextSample);
        }
    }
}
