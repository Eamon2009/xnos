#pragma once

#include <string>

namespace iso_kernos {

std::string readTextFile(const std::string& path);
bool readFirstLine(const std::string& path, std::string& out);

} // namespace iso_kernos
