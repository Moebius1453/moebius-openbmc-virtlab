#pragma once

#include <xyz/openbmc_project/Metric/Value/server.hpp>
#include <sdbusplus/server/object.hpp>
#include <sys/statvfs.h>
#include <iostream>
using MetricValueIntf =
    sdbusplus::server::xyz::openbmc_project::metric::Value;

class StorageInfo :
    public sdbusplus::server::object_t<MetricValueIntf>
{
    // TODO：构造函数和采集更新
    public:
    enum class Mode
    {
        Total,
        Available
    };

    StorageInfo(sdbusplus::bus_t& bus, const char* path, Mode mode) :
        sdbusplus::server::object_t<MetricValueIntf>(bus, path),
        mode_(mode)
    {
        unit(MetricValueIntf::Unit::Bytes);
    }
    void update()
    {
        struct statvfs info {};

        if (statvfs("/run/initramfs/rw", &info) != 0)
        {
            std::cerr << "[storageinfo] failed to read filesystem statistics"
                    << std::endl;
            return;
        }

        // TODO：计算容量，再根据 mode_ 发布对应数值
        const auto totalBytes =
        static_cast<unsigned long long>(info.f_blocks) * info.f_frsize;

        const auto availableBytes =
            static_cast<unsigned long long>(info.f_bavail) * info.f_frsize;

        if (mode_ == Mode::Total)
        {
            value(static_cast<double>(totalBytes));
        }
        else
        {
            value(static_cast<double>(availableBytes));
        }

    }
    
    


    private:
    Mode mode_;


};
