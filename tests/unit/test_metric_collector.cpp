#include "core/MetricCollector.hpp"

#include <stdexcept>

namespace iso_kernos {

void testFormatters();

class MockMonitor final : public IMonitor {
public:
    CPUMetrics getCPU() override { return CPUMetrics{12.5, -1.0, -1.0, -1.0}; }
    MemoryMetrics getMemory() override { return MemoryMetrics{50.0, 100, 50, 50}; }
    GPUMetrics getGPU() override { return GPUMetrics{1.0, 2.0, 4.0, "mock"}; }
};

void testMetricCollector()
{
    MockMonitor monitor;
    MetricCollector collector(monitor);
    const SystemMetrics metrics = collector.collect();
    if (metrics.cpu.usagePercent != 12.5 || metrics.memory.totalMiB != 100 || metrics.gpu.name != "mock") {
        throw std::runtime_error("collector did not aggregate expected metrics");
    }
}

} // namespace iso_kernos

int main()
{
    iso_kernos::testFormatters();
    iso_kernos::testMetricCollector();
    return 0;
}
