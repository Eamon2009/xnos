#include "display/TerminalUI.hpp"

#include "display/Formatter.hpp"
#include "utils/TimeUtils.hpp"

#include <iomanip>
#include <ostream>
#include <string>

namespace iso_kernos {
namespace {

constexpr int kWidth = 78;

std::string repeat(char c, int n)
{
    return std::string(n > 0 ? n : 0, c);
}

void divider(std::ostream& out, char left, char fill, char right)
{
    out << left << repeat(fill, kWidth - 2) << right << '\n';
}

void row(std::ostream& out, const std::string& a, const std::string& b, const std::string& c, const std::string& d)
{
    out << "| " << std::left << std::setw(18) << a << std::setw(20) << b
        << std::setw(18) << c << std::setw(18) << d << "|\n";
}

void single(std::ostream& out, const std::string& label, const std::string& value)
{
    const int pad = kWidth - 4 - static_cast<int>(label.size()) - static_cast<int>(value.size());
    out << "| " << label << repeat(' ', pad) << value << " |\n";
}

void section(std::ostream& out, const std::string& title)
{
    divider(out, '+', '-', '+');
    single(out, title, "");
    divider(out, '+', '=', '+');
}

} // namespace

TerminalUI::TerminalUI(Config config) : config_(std::move(config)) {}

void TerminalUI::render(const SystemMetrics& metrics, std::ostream& out) const
{
    if (config_.mode == DisplayMode::Json) {
        out << toJson(metrics) << '\n';
    } else if (config_.mode == DisplayMode::Compact) {
        renderCompact(metrics, out);
    } else if (config_.mode == DisplayMode::Detailed) {
        renderDetailed(metrics, out);
    } else {
        renderDashboard(metrics, out);
    }
}

void TerminalUI::renderDashboard(const SystemMetrics& metrics, std::ostream& out) const
{
    out << "\033[2J\033[H";
    divider(out, '+', '-', '+');
    single(out, "iso-kernos hardware monitor", currentDateTime());
    divider(out, '+', '=', '+');
    section(out, "CPU");
    row(out, "Utilization", formatPercent(metrics.cpu.usagePercent), "Load 1m", formatPercent(metrics.cpu.load1, 2));
    section(out, "Memory");
    row(out, "Utilization", formatPercent(metrics.memory.usagePercent), "Total", formatMiB(static_cast<double>(metrics.memory.totalMiB)));
    row(out, "Used", formatMiB(static_cast<double>(metrics.memory.usedMiB)), "Free", formatMiB(static_cast<double>(metrics.memory.freeMiB)));
    section(out, "GPU | " + metrics.gpu.name);
    row(out, "Engine", formatPercent(metrics.gpu.enginePercent), "VRAM Used", formatMiB(metrics.gpu.vramUsedMiB));
    row(out, "VRAM Total", formatMiB(metrics.gpu.vramTotalMiB), "", "");
    divider(out, '+', '-', '+');
}

void TerminalUI::renderCompact(const SystemMetrics& metrics, std::ostream& out) const
{
    out << "CPU: " << formatPercent(metrics.cpu.usagePercent)
        << " | RAM: " << formatPercent(metrics.memory.usagePercent)
        << " | GPU: " << formatPercent(metrics.gpu.enginePercent) << '\n';
}

void TerminalUI::renderDetailed(const SystemMetrics& metrics, std::ostream& out) const
{
    renderDashboard(metrics, out);
    out << "Disk read: " << formatBytesPerSecond(metrics.disk.readBytesPerSecond)
        << " | Disk write: " << formatBytesPerSecond(metrics.disk.writeBytesPerSecond) << '\n'
        << "Network RX: " << formatBytesPerSecond(metrics.network.receiveBytesPerSecond)
        << " | Network TX: " << formatBytesPerSecond(metrics.network.transmitBytesPerSecond) << '\n'
        << "Battery: " << formatPercent(metrics.battery.percent) << " (" << metrics.battery.status << ")\n";
}

} // namespace iso_kernos
