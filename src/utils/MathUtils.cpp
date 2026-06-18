#include "utils/MathUtils.hpp"

namespace iso_kernos {

double percent(double numerator, double denominator)
{
    if (denominator <= 0.0) {
        return -1.0;
    }
    return 100.0 * numerator / denominator;
}

double clampPercent(double value)
{
    if (value < 0.0) {
        return value;
    }
    if (value > 100.0) {
        return 100.0;
    }
    return value;
}

} // namespace iso_kernos
