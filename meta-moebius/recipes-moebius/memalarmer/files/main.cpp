#include <sdbusplus/bus.hpp>
#include <sdbusplus/bus/match.hpp>

#include <iostream>
#include <string>


int main()
{
    // TODO 1：连接系统总线，变量名叫 bus
    auto bus = sdbusplus::bus::new_default();
    // TODO 2：在循环之前创建信号监听对象
    auto rule =
    sdbusplus::match_rules::type::signal() +
    sdbusplus::match_rules::interface(
        "xyz.openbmc_project.Common.Threshold") +
    sdbusplus::match_rules::member("AssertionChanged") +
    sdbusplus::match_rules::path(
        "/xyz/openbmc_project/sensors/utilization/meminfoweb");

    sdbusplus::match alarmMatch
    (
        bus,
        rule,
        [](sdbusplus::message_t& msg)
        {
            // TODO：先准备存放告警内容的变量，再解码消息
            std::string type;
            std::string bound;
            bool asserted = false;
            double value = 0.0;
            msg.read(type, bound, asserted, value);

            std::cout << "[memalarmer] type=" << type
                      << " bound=" << bound
                      << " state=" << (asserted ? "ASSERTED" : "CLEARED")
                      << " value=" << value << "%"
                      << std::endl;
        }
    );

    std::cout << "[memalarmer] started" << std::endl;


    while (true)
    {
        // TODO 3：处理总线消息
        bus.process_discard();
        // TODO 4：等待下一条消息
        bus.wait();
    }

    return 0;
}
