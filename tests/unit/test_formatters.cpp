#include "display/Formatter.hpp"

#include <stdexcept>
#include <string>

#define ASSERT_EQ(actual, expected) \
    do { \
        if ((actual) != (expected)) { \
            throw std::runtime_error(std::string("assertion failed: ") + (actual) + " != " + (expected)); \
        } \
    } while (false)

namespace iso_kernos {

void testFormatters()
{
    ASSERT_EQ(formatPercent(-1.0), "N/A");
    ASSERT_EQ(formatPercent(42.25), "42.2%");
    ASSERT_EQ(formatMiB(128.0), "128 MiB");
}

} // namespace iso_kernos
