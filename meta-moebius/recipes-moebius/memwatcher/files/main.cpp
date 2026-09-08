#include <sdbusplus/bus.hpp>
#include <sdbusplus/server/manager.hpp>

#include "wather.hpp"

int main()
{
    auto bus = sdbusplus::bus::new_default();
   
    MemWatcher memwatcher(bus, "/xyz/moebius/memwatcher");

    while (true)
    {  
        bus.process_discard();
        bus.wait(5000000);
        watcher.check()
    }
    
    return 0;
}