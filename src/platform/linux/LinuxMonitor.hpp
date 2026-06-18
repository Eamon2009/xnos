#pragma once

#include "core/IMonitor.hpp"

namespace iso_kernos {

class LinuxMonitor final : public IMonitor {
public:
    CPUMetrics getCPU() override;
    MemoryMetrics getMemory() override;
    GPUMetrics getGPU() override;
    BatteryMetrics getBattery() override;

private:
    long long previousIdle_ = 0;
    long long previousTotal_ = 0;
    bool firstCpuCall_ = true;
};

} // namespace iso_kernos
