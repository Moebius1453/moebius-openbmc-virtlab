#include "space_status.hpp"

#include <cassert>
#include <initializer_list>
#include <limits>

int main()
{
    using storagewatcher::classify;
    using storagewatcher::SpaceStatus;

    assert(classify(100, 50) == SpaceStatus::Ok);
    assert(classify(100, 100) == SpaceStatus::Ok);
    assert(classify(100, 20.01) == SpaceStatus::Ok);
    assert(classify(100, 20) == SpaceStatus::Warning);
    assert(classify(100, 15) == SpaceStatus::Warning);
    assert(classify(100, 10.01) == SpaceStatus::Warning);
    assert(classify(100, 10) == SpaceStatus::Critical);
    assert(classify(100, 5) == SpaceStatus::Critical);
    assert(classify(100, 0) == SpaceStatus::Critical);
    assert(classify(200, 20) == SpaceStatus::Critical);
    assert(classify(200, 40) == SpaceStatus::Warning);

    assert(classify(0, 0) == SpaceStatus::Unknown);
    assert(classify(-100, 10) == SpaceStatus::Unknown);
    assert(classify(100, -1) == SpaceStatus::Unknown);
    assert(classify(100, 101) == SpaceStatus::Unknown);
    const double nan = std::numeric_limits<double>::quiet_NaN();
    const double inf = std::numeric_limits<double>::infinity();
    for (double invalid : {nan, inf, -inf})
    {
        assert(classify(invalid, 10) == SpaceStatus::Unknown);
        assert(classify(100, invalid) == SpaceStatus::Unknown);
    }
    const double largest = std::numeric_limits<double>::max();
    assert(classify(largest, largest) == SpaceStatus::Ok);
}
