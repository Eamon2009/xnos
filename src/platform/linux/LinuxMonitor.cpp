#include "platform/linux/LinuxMonitor.hpp"

#include "platform/linux/ProcParser.hpp"
#include "platform/linux/SysParser.hpp"
#include "utils/FileReader.hpp"
#include "utils/MathUtils.hpp"

#include <chrono>
#include <thread>

namespace iso_kernos {
namespace {

constexpr int kCpuSampleDelayMs = 250;

bool readCpuSnapshot(long long& idle, long long& total)
{
    return parseProcStat(readTextFile("/proc/stat"), idle, total);
}

} // namespace

CPUMetrics LinuxMonitor::getCPU()
{
    CPUMetrics metrics;
    long long idle = 0;
    long long total = 0;
    if (!readCpuSnapshot(idle, total)) {
        return metrics;
    }
    if (firstCpuCall_) {
        previousIdle_ = idle;
        previousTotal_ = total;
        firstCpuCall_ = false;
        std::this_thread::sleep_for(std::chrono::milliseconds(kCpuSampleDelayMs));
        if (!readCpuSnapshot(idle, total)) {
            return metrics;
        }
    }
    const long long deltaIdle = idle - previousIdle_;
    const long long deltaTotal = total - previousTotal_;
    previousIdle_ = idle;
    previousTotal_ = total;
    if (deltaTotal > 0) {
        metrics.usagePercent = clampPercent(100.0 * (1.0 - static_cast<double>(deltaIdle) / deltaTotal));
    }
    parseLoadAverage(readTextFile("/proc/loadavg"), metrics.load1, metrics.load5, metrics.load15);
    return metrics;
}

MemoryMetrics LinuxMonitor::getMemory()
{
    MemoryMetrics metrics;
    const std::string memInfo = readTextFile("/proc/meminfo");
    const long long totalKiB = parseMemInfoKiB(memInfo, "MemTotal:");
    const long long availableKiB = parseMemInfoKiB(memInfo, "MemAvailable:");
    if (totalKiB <= 0 || availableKiB < 0) {
        return metrics;
    }
    const long long usedKiB = totalKiB - availableKiB;
    metrics.totalMiB = totalKiB / 1024;
    metrics.usedMiB = usedKiB / 1024;
    metrics.freeMiB = availableKiB / 1024;
    metrics.usagePercent = percent(static_cast<double>(usedKiB), static_cast<double>(totalKiB));
    return metrics;
}

GPUMetrics LinuxMonitor::getGPU()
{
    return readLinuxGpu();
}

BatteryMetrics LinuxMonitor::getBattery()
{
    return readLinuxBattery();
}

} // namespace iso_kernos
