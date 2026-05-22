#include "platform/linux/SysParser.hpp"

#include "utils/FileReader.hpp"
#include "utils/StringUtils.hpp"

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>

namespace iso_kernos {
namespace {

double readDoubleFile(const std::string& path)
{
    std::string line;
    if (!readFirstLine(path, line)) {
        return -1.0;
    }
    char* end = nullptr;
    const double value = std::strtod(line.c_str(), &end);
    return end == line.c_str() ? -1.0 : value;
}

void readDrmVram(GPUMetrics& metrics, int card)
{
    const std::string base = "/sys/class/drm/card" + std::to_string(card) + "/device/";
    const double used = readDoubleFile(base + "mem_info_vram_used");
    const double total = readDoubleFile(base + "mem_info_vram_total");
    if (used >= 0.0 && total > 0.0) {
        metrics.vramUsedMiB = used / (1024.0 * 1024.0);
        metrics.vramTotalMiB = total / (1024.0 * 1024.0);
    }
}

void readDrmName(GPUMetrics& metrics, int card)
{
    std::ifstream file("/sys/class/drm/card" + std::to_string(card) + "/device/uevent");
    std::string line;
    while (std::getline(file, line)) {
        if (line.rfind("DRIVER=", 0) == 0) {
            metrics.name = line.substr(7);
            return;
        }
    }
}

GPUMetrics readNvidiaSmi()
{
    GPUMetrics metrics;
    FILE* pipe = ::popen(
        "nvidia-smi --query-gpu=utilization.gpu,memory.used,memory.total,name "
        "--format=csv,noheader,nounits 2>/dev/null",
        "r");
    if (!pipe) {
        return metrics;
    }
    char buffer[512] = {};
    if (std::fgets(buffer, sizeof(buffer), pipe)) {
        std::istringstream stream(buffer);
        std::string token;
        int field = 0;
        while (std::getline(stream, token, ',')) {
            token = trim(token);
            try {
                if (field == 0) {
                    metrics.enginePercent = std::stod(token);
                } else if (field == 1) {
                    metrics.vramUsedMiB = std::stod(token);
                } else if (field == 2) {
                    metrics.vramTotalMiB = std::stod(token);
                } else if (field == 3) {
                    metrics.name = token;
                }
            } catch (...) {
            }
            ++field;
        }
    }
    ::pclose(pipe);
    return metrics;
}

} // namespace

GPUMetrics readLinuxGpu()
{
    for (int card = 0; card < 8; ++card) {
        const std::string base = "/sys/class/drm/card" + std::to_string(card) + "/device/";
        const double busy = readDoubleFile(base + "gpu_busy_percent");
        if (busy >= 0.0) {
            GPUMetrics metrics;
            metrics.enginePercent = busy;
            readDrmVram(metrics, card);
            readDrmName(metrics, card);
            return metrics;
        }
    }
    return readNvidiaSmi();
}

BatteryMetrics readLinuxBattery()
{
    BatteryMetrics metrics;
    for (int battery = 0; battery < 4; ++battery) {
        const std::string base = "/sys/class/power_supply/BAT" + std::to_string(battery) + "/";
        const double capacity = readDoubleFile(base + "capacity");
        if (capacity < 0.0) {
            continue;
        }
        metrics.percent = capacity;
        std::string status;
        if (readFirstLine(base + "status", status)) {
            metrics.status = trim(status);
        }
        return metrics;
    }
    return metrics;
}

} // namespace iso_kernos
