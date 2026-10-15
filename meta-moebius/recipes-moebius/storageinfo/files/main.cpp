#include "storageinfo.hpp"

#include <sdbusplus/bus.hpp>
#include <sdbusplus/server/manager.hpp>

int main()
{
    auto bus = sdbusplus::bus::new_default();

    bus.request_name("xyz.moebius.StorageInfo");

    sdbusplus::server::manager_t manager(
        bus,
        "/xyz/openbmc_project/metric/bmc/storage/rw"
    );

    // TODO：创建 total 和 available 两个对象

    StorageInfo total(
        bus,
        "/xyz/openbmc_project/metric/bmc/storage/rw/total",
        StorageInfo::Mode::Total
    );

    StorageInfo available(
        bus,
        "/xyz/openbmc_project/metric/bmc/storage/rw/available",
        StorageInfo::Mode::Available
    );


    // TODO：首次采集，再进入事件循环

    total.update();
    available.update();

    while (true)
    {
        bus.process_discard();
        bus.wait(10000000);

        total.update();
        available.update();
    }


    return 0;
}
