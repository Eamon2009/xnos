#include "utils/FileReader.hpp"

#include <fstream>
#include <sstream>

namespace iso_kernos {

std::string readTextFile(const std::string& path)
{
    std::ifstream file(path);
    if (!file) {
        return {};
    }
    std::ostringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

bool readFirstLine(const std::string& path, std::string& out)
{
    std::ifstream file(path);
    return static_cast<bool>(std::getline(file, out));
}

} // namespace iso_kernos
