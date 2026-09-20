#pragma once

#include <xyz/openbmc_project/Sensor/Value/server.hpp>
#include <xyz/openbmc_project/Association/Definitions/server.hpp>
#include <xyz/openbmc_project/Common/Threshold/server.hpp>
#include <sdbusplus/bus.hpp>
#include <variant>
#include <string>

using SensorValueIntf = sdbusplus::server::xyz::openbmc_project::sensor::Value;
using AssocDefIntf    = sdbusplus::server::xyz::openbmc_project::association::Definitions;
using ThresholdIntf   = sdbusplus::server::xyz::openbmc_project::common::Threshold;

class MemInfoWeb : public sdbusplus::server::object_t<SensorValueIntf, AssocDefIntf, ThresholdIntf>
{
    public:
        MemInfoWeb(sdbusplus::bus_t& bus, const char* path) :
            sdbusplus::server::object_t<SensorValueIntf, AssocDefIntf, ThresholdIntf>(bus, path),
            bus_(bus)
        {
            unit(SensorValueIntf::Unit::Percent);        // 单位：百分比

            associations({                               // 挂归属纸条
                {"chassis", "all_sensors",
                 "/xyz/openbmc_project/inventory/system/chassis"}
            });
            ThresholdIntf::value({
                {Type::Warning,  {{Bound::Upper, 30.0}}},
                {Type::Critical, {{Bound::Upper, 95.0}}},
            });
        }

        void update()
        {
            double used  = readMetric("/xyz/openbmc_project/metric/bmc/memory/used");
            double total = readMetric("/xyz/openbmc_project/metric/bmc/memory/total");

            if (total <= 0)                               
            {
                return;             
            }

            double pct = used / total * 100.0;
            SensorValueIntf::value(pct);

            bool over = pct > 30.0;
            if(over != warningAsserted_)
            {
                warningAsserted_ = over;
                if(over)
                {
                    asserted({{Type::Warning, Bound::Upper}});
                }
                else
                {
                    asserted({});

                }
                assertionChanged(Type::Warning, Bound::Upper, over, pct);
            }

        }

    private:
        using Type  = ThresholdIntf::Type;
        using Bound = ThresholdIntf::Bound;
        bool warningAsserted_ = false;
        sdbusplus::bus_t& bus_;      // 记住总线 主动发请求

        double readMetric(const std::string& objectPath)
        {
            auto method = bus_.new_method_call(
                "xyz.moebius.MemInfo", objectPath,
                "org.freedesktop.DBus.Properties", "Get");
            method.append("xyz.openbmc_project.Metric.Value", "Value");

            auto reply = bus_.call(method);

            std::variant<double> v; 
            reply.read(v);                       
            return std::get<double>(v);
        }
};
