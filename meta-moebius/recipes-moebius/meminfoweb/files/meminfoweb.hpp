#pragma once

#include <xyz/openbmc_project/Sensor/Value/server.hpp>
#include <xyz/openbmc_project/Association/Definitions/server.hpp>
#include <sdbusplus/bus.hpp>
#include <variant>
#include <string>

using SensorValueIntf = sdbusplus::server::xyz::openbmc_project::sensor::Value;
using AssocDefIntf    = sdbusplus::server::xyz::openbmc_project::association::Definitions;

class MemInfoWeb : public sdbusplus::server::object_t<SensorValueIntf, AssocDefIntf>
{
    public:
        MemInfoWeb(sdbusplus::bus_t& bus, const char* path) :
            sdbusplus::server::object_t<SensorValueIntf, AssocDefIntf>(bus, path),
            bus_(bus)
        {
            unit(SensorValueIntf::Unit::Percent);        // 单位：百分比

            associations({                               // 挂归属纸条
                {"chassis", "all_sensors",
                 "/xyz/openbmc_project/inventory/system/chassis"}
            });
        }

        void update()
        {
            double used  = readMetric("/xyz/openbmc_project/metric/bmc/memory/used");
            double total = readMetric("/xyz/openbmc_project/metric/bmc/memory/total");

            if (total > 0)                               // 除零防护
            {
                value(used / total * 100.0);             // 推百分比
            }
        }

    private:
        sdbusplus::bus_t& bus_;      // 记住总线（这次要主动发请求）

        double readMetric(const std::string& objectPath)
        {
            auto method = bus_.new_method_call(
                "xyz.moebius.MemInfo", objectPath,
                "org.freedesktop.DBus.Properties", "Get");
            method.append("xyz.openbmc_project.Metric.Value", "Value");

            auto reply = bus_.call(method);      // 同步：发出去、等回复

            std::variant<double> v;              // 万能箱子
            reply.read(v);                       // 拆包
            return std::get<double>(v);          // 取出 double
        }
};
