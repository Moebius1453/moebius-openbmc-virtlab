#pragma once

#include <sdbusplus/bus.hpp>
#include <cstdint>
#include <iostream>
#include <variant>
#include <map>
#include <vector>
#include <string>

class MemWatcher
{
  public:
    MemWatcher(sdbusplus::bus_t& bus) : bus(bus) {}

    void check()
    {
        uint64_t avail = readMemUsed();
        if (!hasLast)
        {
            std::cout << "memwatcher: baseline used = " << avail
                      << " Byte" << std::endl;
        }
        else
        {
            uint64_t diff = (avail > last) ? (avail - last) : (last - avail);
            if (diff > THRESHOLD)
            {
                std::cout << "memwatcher: used changed "
                          << last << " Byte -> " << avail << " Byte" << std::endl;
            }
        }
        last = avail;
        hasLast = true;
    }
    void onSignal(sdbusplus::message_t& msg)
    {
        std::string interfaceName;
        std::map<std::string, std::variant<double>> changed;
        std::vector<std::string> invalidated;
        msg.read(interfaceName, changed, invalidated);

        auto it = changed.find("Value");
        if (it != changed.end())
        {
            std::cout << "memwatcher: HealthMon cpu/user = "
                      << std::get<double>(it->second) << std::endl;
        }
    }

  private:
    static constexpr uint64_t THRESHOLD = 10 * 1024 * 1024; // ±10MB；属性单位是 字节，所以 10MB = 10485760

    uint64_t readMemUsed()
    {
        auto method = bus.new_method_call(
            "xyz.moebius.MemInfo", "/xyz/openbmc_project/metric/bmc/memory/used",
            "org.freedesktop.DBus.Properties", "Get");
        method.append("xyz.openbmc_project.Metric.Value", "Value");
        auto reply = bus.call(method);
        std::variant<double> value;
        reply.read(value);
        return static_cast<uint64_t>(std::get<double>(value));

    }

    sdbusplus::bus_t& bus;
    uint64_t last = 0;
    bool hasLast = false;
};
