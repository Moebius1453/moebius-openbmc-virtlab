#include "healthreporter.hpp"


int main()                          
{
    auto bus = sdbusplus::bus::new_default();
    bus.request_name("xyz.moebius.HealthReporter");

    sdbusplus::server::manager_t objManager(bus, "/xyz/openbmc_project/sensors/utilization");
    HealthReporter rep(bus, "/xyz/openbmc_project/sensors/utilization/healthreporter");

    while (true)
    {
        bus.process_discard();
        bus.wait(10000000);        
        rep.update();
    }
    return 0;
}
