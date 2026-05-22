#include "utils/TimeUtils.hpp"

#include <ctime>

namespace iso_kernos {

std::string currentDateTime()
{
    std::time_t now = std::time(nullptr);
    char buffer[64] = {};
    std::tm localTime{};
#if defined(_WIN32)
    localtime_s(&localTime, &now);
#else
    localtime_r(&now, &localTime);
#endif
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &localTime);
    return buffer;
}

} // namespace iso_kernos
