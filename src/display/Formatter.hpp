#pragma once

#include "core/MetricTypes.hpp"

#include <string>

namespace iso_kernos {

std::string formatPercent(double value, int precision = 1);
std::string formatMiB(double value, int precision = 0);
std::string formatBytesPerSecond(double value);
std::string toJson(const SystemMetrics& metrics);

} // namespace iso_kernos
