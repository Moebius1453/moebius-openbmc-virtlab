#pragma once

#include <xyz/moebius/MemoryStats/server.hpp>
#include <sdbusplus/server/object.hpp>
#include <cstdint>   
#include <fstream> 
#include <string>

using MemoryStatsIntf =
    sdbusplus::xyz::moebius::MemoryStats::server::MemoryStats;
class MemInfo : public sdbusplus::server::object_t<MemoryStatsIntf>
{
    public:
        MemInfo(sdbusplus::bus_t& bus, const char* path) :
            sdbusplus::server::object_t<MemoryStatsIntf>(bus, path)
        {}
    uint64_t memTotal(uint64_t value) override
    {
        return readProcMeminfo("MemTotal");
    }
    uint64_t memAvailable(uint64_t value) override
    {
        return readProcMeminfo("MemAvailable");
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

