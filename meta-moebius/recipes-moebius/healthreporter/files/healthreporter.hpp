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
            // ---- 1. 读源 ----
            std::string bmcState = tail(readStrProp(
                "xyz.openbmc_project.State.BMC", "/xyz/openbmc_project/state/bmc0",
                "xyz.openbmc_project.State.BMC", "CurrentBMCState"));
            // TODO: hostState ← State.Host / host0 / State.Host / CurrentHostState
            std::string hostState = tail(readStrProp(
                "xyz.openbmc_project.State.Host", "/xyz/openbmc_project/state/host0",
                "xyz.openbmc_project.State.Host", "CurrentHostState"));
            // TODO: osState   ← State.Host / host0 / State.OperatingSystem.Status / OperatingSystemState
            std::string osState = tail(readStrProp(
                "xyz.openbmc_project.State.Host", "/xyz/openbmc_project/state/host0",
                "xyz.openbmc_project.State.OperatingSystem.Status", "OperatingSystemState"));
            // TODO: powerState← State.Chassis / chassis0 / State.Chassis / CurrentPowerState
            std::string powerState = tail(readStrProp(
                "xyz.openbmc_project.State.Chassis", "/xyz/openbmc_project/state/chassis0",
                "xyz.openbmc_project.State.Chassis", "CurrentPowerState"));
            // TODO: lastReboot ← readU64Prop(... State.BMC ... LastRebootTime)
            uint64_t lastReboot = readU64Prop(
                "xyz.openbmc_project.State.BMC", "/xyz.openbmc_project/state/bmc0",
                "xyz.openbmc_project.State.BMC", "LastRebootTime");
            // TODO: used, total ← 照 meminfoweb 的 readMetric 写（double）
            double used  = readMetric("/xyz/openbmc_project/metric/bmc/memory/used");
            double total = readMetric("/xyz/openbmc_project/metric/bmc/memory/total");

            auto nowMs = std::chrono::duration_cast<std::chrono::milliseconds>(
                             std::chrono::system_clock::now().time_since_epoch()).count();
            uint64_t uptimeSec = (nowMs - lastReboot) / 1000;
            
            // ---- 2. 判级（规则）：----
            // OK 起步；bmcState != "Ready" → Critical；memPct>95 → Critical；
            // memPct>80 且还是 OK → Warning
            std::string level = "OK";
            // TODO: 三条判定
            double memPct = used / total * 100.0;

            if (bmcState != "Ready")
            {
                level = "Critical";
            }

            if (memPct > 95.0)
            {
                level = "Critical";
            }
            else if (memPct > 80.0 && level == "OK")
            {
                level = "Warning";
            }

            // ---- 3. 写 D-Bus ----
            // score = OK?100 : Warning?50 : 0 ；functional = (level != "Critical")

            // TODO: SensorValueIntf::value(score);  OpStatusIntf::functional(...);
            double score = (level == "OK") ? 100.0 : ((level == "Warning") ? 50.0 : 0.0);

            SensorValueIntf::value(score);
            OpStatusIntf::functional(level != "Critical");

            // ---- 4. journal ----
            // TODO: 拼一行 cout（把 uptime、memPct、level、host/os/power 都打出来）
            std::cout << "[HealthReporter] level=" << level
                      << " score=" << score
                      << " uptime=" << uptimeSec << "s"
                      << " mem=" << memPct << "%"
                      << " host=" << hostState
                      << " os=" << osState
                      << " power=" << powerState
                      << std::endl;
                   
        }

    private:
        sdbusplus::bus_t& bus_;

        std::string readStrProp(const std::string& service, const std::string& path,
                                const std::string& iface, const std::string& prop)
        {
            std::string svc = service;
            std::string pth = path;
            std::cout << "[DBG] str: [" << service << "] [" << path << "] [" << iface << "] [" << prop << "]" << std::endl;
            auto method = bus_.new_method_call(svc.c_str(), pth.c_str(),
                                   "org.freedesktop.DBus.Properties", "Get");
            

            method.append(iface, prop);
            auto reply = bus_.call(method);
            std::variant<std::string> v;
            reply.read(v);
            return std::get<std::string>(v);
        }
        uint64_t readU64Prop(const std::string& service, const std::string& path,
                                const std::string& iface, const std::string& prop)
        {
            std::string svc = service;
            std::string pth = path;
            std::cout << "[DBG] u64-1 enter" << std::endl;
            auto method = bus_.new_method_call(
                "xyz.openbmc_project.State.BMC",
                "/xyz/openbmc_project/state/bmc0",
                "org.freedesktop.DBus.Properties", "Get");
            std::cout << "[DBG] u64-2 method-created" << std::endl;
            method.append(iface, prop);
            std::cout << "[DBG] u64-3 appended" << std::endl;
            auto reply = bus_.call(method);
            std::cout << "[DBG] u64-4 called" << std::endl;
            std::variant<uint64_t> v;
            reply.read(v);
            std::cout << "[DBG] u64-5 read-ok value=" << std::get<uint64_t>(v) << std::endl;
            return std::get<uint64_t>(v);
        }

        
        double readMetric(const std::string& objectPath)
        {
            std::cout << "[DBG] metric enter [" << objectPath << "]" << std::endl;
            auto method = bus_.new_method_call(
                "xyz.moebius.MemInfo", objectPath,
                "org.freedesktop.DBus.Properties", "Get");

            
                
            method.append("xyz.openbmc_project.Metric.Value", "Value");

            auto reply = bus_.call(method);

            std::variant<double> v;
            reply.read(v);
            return std::get<double>(v);
        }

        static std::string tail(const std::string& s) { return s.substr(s.rfind('.') + 1); }

        

         


};




