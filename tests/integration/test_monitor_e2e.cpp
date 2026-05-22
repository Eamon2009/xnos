#include "core/MetricCollector.hpp"
#include "core/MonitorFactory.hpp"

#include <stdexcept>

int main()
{
    auto monitor = iso_kernos::MonitorFactory::create();
    if (!monitor) {
        throw std::runtime_error("no monitor for this platform");
    }
    iso_kernos::MetricCollector collector(*monitor);
    (void)collector.collect();
    return 0;
}
