#include "platform/macos/MacOSMonitor.hpp"

#if defined(__APPLE__)

#include <mach/mach.h>
#include <sys/sysctl.h>

namespace iso_kernos {

CPUMetrics MacOSMonitor::getCPU()
{
    return {};
}

MemoryMetrics MacOSMonitor::getMemory()
{
    return {};
}

GPUMetrics MacOSMonitor::getGPU()
{
    return {};
}

} // namespace iso_kernos

#endif
