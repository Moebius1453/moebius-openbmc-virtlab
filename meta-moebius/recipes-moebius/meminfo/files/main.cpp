#include <sdbusplus/bus.hpp>
#include <sdbusplus/server/manager.hpp>

#include "meminfo.hpp"

int main()
{
    auto bus = sdbusplus::bus::new_default();
    bus.request_name("xyz.moebius.MemInfo");

    sdbusplus::server::manager_t objManager(
        bus, "/xyz/openbmc_project/metric/bmc/memory");
    MemInfo memInfo(bus, "/xyz/openbmc_project/metric/bmc/memory/used");

    while (true)
    {  
        bus.process_discard();
        bus.wait(5000000);
        memInfo.update();
    }
    
    return 0;
}