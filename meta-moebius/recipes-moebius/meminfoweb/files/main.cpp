#include <sdbusplus/bus.hpp>
#include <sdbusplus/server/manager.hpp>

#include "meminfoweb.hpp"

int main()
{
    auto bus = sdbusplus::bus::new_default();
    bus.request_name("xyz.moebius.MemInfoWeb");

    sdbusplus::server::manager_t objManager(
        bus, "/xyz/openbmc_project/sensors/utilization");
    MemInfoWeb sensor(bus, "/xyz/openbmc_project/sensors/utilization/meminfoweb");

    while (true)
    {
        bus.process_discard();
        bus.wait(5000000);
        sensor.update();
    }
    return 0;
}
