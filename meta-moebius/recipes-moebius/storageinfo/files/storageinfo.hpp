#pragma once

#include <xyz/openbmc_project/Metric/Value/server.hpp>
#include <sdbusplus/server/object.hpp>

using MetricValueIntf =
    sdbusplus::server::xyz::openbmc_project::metric::Value;

class StorageInfo :
    public sdbusplus::server::object_t<MetricValueIntf>
{
    // TODO：构造函数和采集更新
    public:
    StorageInfo(sdbusplus::bus_t& bus, const char* path) :
        sdbusplus::server::object_t<MetricValueIntf>(bus, path)
    {
        unit(MetricValueIntf::Unit::Bytes);
    }

};
