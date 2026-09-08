#include <sdbusplus/bus.hpp>
#include <sdbusplus/server/manager.hpp>

#include "watcher.hpp"

int main()
{
    auto bus = sdbusplus::bus::new_default();
   
    MemWatcher memwatcher(bus);

    while (true)
    {  
        bus.process_discard();
        bus.wait(5000000);
        memwatcher.check();
    }
    
    return 0;
}