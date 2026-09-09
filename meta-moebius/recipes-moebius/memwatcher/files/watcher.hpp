#pragma once

#include <sdbusplus/bus.hpp>
#include <cstdint>
#include <iostream>
#include <variant>
#include <map>
#include <vector>
#include <string>

class MemWatcher
{
  public:
    MemWatcher(sdbusplus::bus_t& bus) : bus(bus) {}

    void check()
    {
        uint64_t avail = readMemAvailable();
        if (!hasLast)
        {
            std::cout << "memwatcher: baseline MemAvailable = " << avail
                      << " kB" << std::endl;
        }
        else
        {
            uint64_t diff = (avail > last) ? (avail - last) : (last - avail);
            if (diff > THRESHOLD)
            {
                std::cout << "memwatcher: MemAvailable changed "
                          << last << " kB -> " << avail << " kB" << std::endl;
            }
        }
        last = avail;
        hasLast = true;
    }
    void onSignal(sdbusplus::message_t& msg)
    {
        std::string interfaceName;
        std::map<std::string, std::variant<double>> changed;
        std::vector<std::string> invalidated;
        msg.read(interfaceName, changed, invalidated);

        auto it = changed.find("Value");
        if (it != changed.end())
        {
            std::cout << "memwatcher: HealthMon cpu/user = "
                      << std::get<double>(it->second) << std::endl;
        }
    }

  private:
    static constexpr uint64_t THRESHOLD = 10 * 1024; // ±10MB；属性单位是 kB，所以 10MB = 10240

    uint64_t readMemAvailable()
    {
        auto method = bus.new_method_call(
            "xyz.moebius.MemInfo", "/xyz/moebius/meminfo",
            "org.freedesktop.DBus.Properties", "Get");
        method.append("xyz.moebius.MemoryStats", "MemAvailable");
        auto reply = bus.call(method);
        std::variant<uint64_t> value;
        reply.read(value);
        return std::get<uint64_t>(value);
    }

    sdbusplus::bus_t& bus;
    uint64_t last = 0;
    bool hasLast = false;
};
