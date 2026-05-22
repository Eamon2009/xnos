#pragma once

#include "core/Config.hpp"
#include "core/MetricTypes.hpp"

#include <iosfwd>

namespace iso_kernos {

class TerminalUI {
public:
    explicit TerminalUI(Config config);

    void render(const SystemMetrics& metrics, std::ostream& out) const;

private:
    void renderDashboard(const SystemMetrics& metrics, std::ostream& out) const;
    void renderCompact(const SystemMetrics& metrics, std::ostream& out) const;
    void renderDetailed(const SystemMetrics& metrics, std::ostream& out) const;

    Config config_;
};

} // namespace iso_kernos
