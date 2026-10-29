#pragma once

#include <cmath>

namespace storagewatcher
{
enum class SpaceStatus
{
    Unknown,
    Critical,
    Warning,
    Ok,
};

// 只判断本次读到的两个值，不把无效数据当成空间充足。
inline SpaceStatus classify(double total, double available)
{
    if (!std::isfinite(total) || !std::isfinite(available) || total <= 0 ||
        available < 0 || available > total)
    {
        return SpaceStatus::Unknown;
    }

    // 先除后乘，避免大容量值乘以 100 时溢出。
    const double percent = (available / total) * 100.0;
    if (percent <= 10.0)
    {
        return SpaceStatus::Critical;
    }
    if (percent <= 20.0)
    {
        return SpaceStatus::Warning;
    }
    return SpaceStatus::Ok;
}
} // namespace storagewatcher
