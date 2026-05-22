#include "alerts/AlertManager.hpp"
#include "alerts/Logger.hpp"
#include "core/Config.hpp"
#include "core/MetricCollector.hpp"
#include "core/MonitorFactory.hpp"
#include "display/TerminalUI.hpp"

#include <chrono>
#include <iostream>
#include <thread>

int main(int argc, char** argv)
{
    iso_kernos::Config config;
    std::string error;
    if (!iso_kernos::applyCommandLine(argc, argv, config, error)) {
        if (error == "help") {
            std::cout << iso_kernos::usageText();
            return 0;
        }
        if (error == "version") {
            std::cout << "iso-kernos 1.0.0\n";
            return 0;
        }
        std::cerr << error << "\n\n" << iso_kernos::usageText();
        return 2;
    }

    auto monitor = iso_kernos::MonitorFactory::create();
    if (!monitor) {
        std::cerr << "Unsupported platform\n";
        return 1;
    }

    iso_kernos::MetricCollector collector(*monitor);
    iso_kernos::TerminalUI ui(config);
    iso_kernos::AlertManager alerts(config);

    const auto start = std::chrono::steady_clock::now();
    do {
        const iso_kernos::SystemMetrics metrics = collector.collect();
        ui.render(metrics, std::cout);
        iso_kernos::appendJsonLog(config.logFile, metrics);
        for (const auto& alert : alerts.evaluate(metrics)) {
            std::cerr << "alert: " << alert.metric << " " << alert.value
                      << "% >= " << alert.threshold << "%\n";
        }

        if (config.testMode) {
            return 0;
        }
        if (config.durationSeconds >= 0) {
            const auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(
                std::chrono::steady_clock::now() - start);
            if (elapsed.count() >= config.durationSeconds) {
                return 0;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(config.refreshRateMs));
    } while (true);
}
