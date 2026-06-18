#pragma once

#include "core/IMonitor.hpp"

namespace iso_kernos {

class MacOSMonitor final : public IMonitor {
public:
    CPUMetrics getCPU() override;
    MemoryMetrics getMemory() override;
    GPUMetrics getGPU() override;
};

} // namespace iso_kernos
