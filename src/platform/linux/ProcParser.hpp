#pragma once

#include <string>

namespace iso_kernos {

bool parseProcStat(const std::string& content, long long& idle, long long& total);
long long parseMemInfoKiB(const std::string& content, const std::string& key);
bool parseLoadAverage(const std::string& content, double& one, double& five, double& fifteen);

} // namespace iso_kernos
