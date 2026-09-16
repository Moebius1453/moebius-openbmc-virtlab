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
        MemInfo(sdbusplus::bus_t& bus, const char* path) :
            sdbusplus::server::object_t<MetricValueIntf>(bus, path)
        {}
    void update()
{
    uint64_t total     = readProcMeminfo("MemTotal");
    uint64_t available = readProcMeminfo("MemAvailable");

    value( (total - available) * 1024 );
}

    private:
   
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



