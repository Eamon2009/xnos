#include "platform/linux/ProcParser.hpp"

#include <sstream>

namespace iso_kernos {

bool parseProcStat(const std::string& content, long long& outIdle, long long& outTotal)
{
    std::istringstream input(content);
    std::string tag;
    input >> tag;
    if (tag != "cpu") {
        return false;
    }
    long long user = 0;
    long long nice = 0;
    long long system = 0;
    long long idle = 0;
    long long iowait = 0;
    long long irq = 0;
    long long softirq = 0;
    long long steal = 0;
    input >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;
    if (!input) {
        return false;
    }
    outIdle = idle + iowait;
    outTotal = user + nice + system + idle + iowait + irq + softirq + steal;
    return true;
}

long long parseMemInfoKiB(const std::string& content, const std::string& key)
{
    std::istringstream input(content);
    std::string line;
    while (std::getline(input, line)) {
        if (line.rfind(key, 0) == 0) {
            std::istringstream value(line.substr(key.size()));
            long long parsed = -1;
            value >> parsed;
            return parsed;
        }
    }
    return -1;
}

bool parseLoadAverage(const std::string& content, double& one, double& five, double& fifteen)
{
    std::istringstream input(content);
    input >> one >> five >> fifteen;
    return static_cast<bool>(input);
}

} // namespace iso_kernos
