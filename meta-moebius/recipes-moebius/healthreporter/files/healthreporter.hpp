#pragma once
#include <xyz/openbmc_project/Sensor/Value/server.hpp>
#include <xyz/openbmc_project/State/Decorator/OperationalStatus/server.hpp>
#include <xyz/openbmc_project/Association/Definitions/server.hpp>
#include <sdbusplus/bus.hpp>
#include <iostream>
#include <chrono>
#include <string>                 
#include <variant>


using SensorValueIntf = sdbusplus::server::xyz::openbmc_project::sensor::Value;
using OpStatusIntf = sdbusplus::server::xyz::openbmc_project::state::decorator::OperationalStatus;
using AssocDefIntf = sdbusplus::server::xyz::openbmc_project::association::Definitions;

class HealthReporter : public sdbusplus::server::object_t<SensorValueIntf, OpStatusIntf, AssocDefIntf>
{
    public:
        HealthReporter(sdbusplus::bus_t& bus, const char* path) :
            sdbusplus::server::object_t<SensorValueIntf, OpStatusIntf, AssocDefIntf>(bus, path),
            bus_(bus)
        {
            unit(SensorValueIntf::Unit::Percent);   
            associations({                          
                {"chassis", "all_sensors",
                 "/xyz/openbmc_project/inventory/system/chassis"}
            });
        }
    
        

    void update()
        {
            // ---- 1. 读源（例子给了一个，其余照葫芦画瓢）----
            std::string bmcState = tail(readStrProp(
                "xyz.openbmc_project.State.BMC", "/xyz/openbmc_project/state/bmc0",
                "xyz.openbmc_project.State.BMC", "CurrentBMCState"));
            // TODO: hostState ← State.Host / host0 / State.Host / CurrentHostState
            // TODO: osState   ← State.Host / host0 / State.OperatingSystem.Status / OperatingSystemState
            // TODO: powerState← State.Chassis / chassis0 / State.Chassis / CurrentPowerState
            // TODO: lastReboot ← readU64Prop(... State.BMC ... LastRebootTime)
            // TODO: used, total ← 照 meminfoweb 的 readMetric 写（double）

            // ---- 2. 判级（规则）：----
            // OK 起步；bmcState != "Ready" → Critical；memPct>95 → Critical；
            // memPct>80 且还是 OK → Warning
            std::string level = "OK";
            // TODO: 三条判定

            // ---- 3. 写 D-Bus ----
            // score = OK?100 : Warning?50 : 0 ；functional = (level != "Critical")
            // TODO: SensorValueIntf::value(score);  OpStatusIntf::functional(...);

            // ---- 4. journal（③④ 这两条搬进这里）----
            // TODO: 拼一行 cout（把 uptime、memPct、level、host/os/power 都打出来）
        }

    private:
        sdbusplus::bus_t& bus_;

        std::string readStrProp(const std::string& service, const std::string& path,
                                const std::string& iface, const std::string& prop)
        {
            auto method = bus_.new_method_call(service.c_str(), path.c_str(),
                                   "org.freedesktop.DBus.Properties", "Get");

            method.append(iface, prop);
            auto reply = bus_.call(method);
            std::variant<std::string> v;
            reply.read(v);
            return std::get<std::string>(v);
        }


        static std::string tail(const std::string& s) { return s.substr(s.rfind('.') + 1); }

        

         


};




