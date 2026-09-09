#include <sdbusplus/bus.hpp>
#include <sdbusplus/bus/match.hpp>
#include "watcher.hpp"

int main()
{
    auto bus = sdbusplus::bus::new_default();
   
    MemWatcher memwatcher(bus);
    sdbusplus::match cpuMatch(
        bus,
        sdbusplus::match_rules::type::signal() +
            sdbusplus::match_rules::interface("org.freedesktop.DBus.Properties") +
            sdbusplus::match_rules::member("PropertiesChanged") +
            sdbusplus::match_rules::path(
                "/xyz/openbmc_project/metric/bmc/cpu/user"),
        [&memwatcher](sdbusplus::message_t& msg) 
        {
            memwatcher.onSignal(msg);
        }
    );
    while (true)
    {  
        bus.process_discard();
        bus.wait(5000000);
        memwatcher.check();
    }
    
    return 0;
}