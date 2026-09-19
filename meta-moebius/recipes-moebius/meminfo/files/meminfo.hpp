#pragma once

#include <xyz/openbmc_project/Metric/Value/server.hpp>
#include <sdbusplus/server/object.hpp>
#include <cstdint>   
#include <fstream> 
#include <string>

using MetricValueIntf = sdbusplus::server::xyz::openbmc_project::metric::Value;

class MemInfo : public sdbusplus::server::object_t<MetricValueIntf>
{
    public:
        enum class Mode { Used, Total };

        MemInfo(sdbusplus::bus_t& bus, const char* path ,Mode mode) :
            sdbusplus::server::object_t<MetricValueIntf>(bus, path),
            mode_(mode)
        {}
    void update()
{
    uint64_t total     = readProcMeminfo("MemTotal");
    uint64_t available = readProcMeminfo("MemAvailable");

    if (mode_ == Mode::Used)
        {
            value( (total - available) * 1024 );   // 旧算法
        }
        else                                        // 只剩 Total 一种可能
        {
            value( total * 1024 );                  // 新算法
        }
}

    private:
        Mode mode_;                    // 记住"我是哪种"
   
    static uint64_t readProcMeminfo(const std::string& key)
    {
        std::ifstream file("/proc/meminfo");
        std::string line;
        while (std::getline(file, line))
        {
            if (line.starts_with(key))
            {
                return std::stoull(line.substr(key.size() + 2));
            }
        }
        return 0;
    }
};



