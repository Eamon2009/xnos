#include "platform/windows/WindowsMonitor.hpp"

#if defined(_WIN32)

#define WIN32_LEAN_AND_MEAN
#include <dxgi.h>
#include <windows.h>

#include "utils/MathUtils.hpp"

namespace iso_kernos {
namespace {

long long filetimeToLongLong(const FILETIME& value)
{
    ULARGE_INTEGER converted;
    converted.LowPart = value.dwLowDateTime;
    converted.HighPart = value.dwHighDateTime;
    return static_cast<long long>(converted.QuadPart);
}

bool cpuSnapshot(long long& idle, long long& kernel, long long& user)
{
    FILETIME idleFileTime;
    FILETIME kernelFileTime;
    FILETIME userFileTime;
    if (!::GetSystemTimes(&idleFileTime, &kernelFileTime, &userFileTime)) {
        return false;
    }
    idle = filetimeToLongLong(idleFileTime);
    kernel = filetimeToLongLong(kernelFileTime);
    user = filetimeToLongLong(userFileTime);
    return true;
}

} // namespace

WindowsMonitor::WindowsMonitor()
{
    IDXGIFactory1* factory = nullptr;
    if (SUCCEEDED(::CreateDXGIFactory1(__uuidof(IDXGIFactory1), reinterpret_cast<void**>(&factory)))) {
        dxgiFactory_ = factory;
    }
}

WindowsMonitor::~WindowsMonitor()
{
    if (dxgiFactory_) {
        reinterpret_cast<IDXGIFactory1*>(dxgiFactory_)->Release();
        dxgiFactory_ = nullptr;
    }
}

CPUMetrics WindowsMonitor::getCPU()
{
    CPUMetrics metrics;
    long long idle = 0;
    long long kernel = 0;
    long long user = 0;
    if (!cpuSnapshot(idle, kernel, user)) {
        return metrics;
    }
    if (firstCpuCall_) {
        previousIdle_ = idle;
        previousKernel_ = kernel;
        previousUser_ = user;
        firstCpuCall_ = false;
        ::Sleep(250);
        if (!cpuSnapshot(idle, kernel, user)) {
            return metrics;
        }
    }
    const long long deltaIdle = idle - previousIdle_;
    const long long deltaKernel = kernel - previousKernel_;
    const long long deltaUser = user - previousUser_;
    previousIdle_ = idle;
    previousKernel_ = kernel;
    previousUser_ = user;

    const long long busy = (deltaKernel - deltaIdle) + deltaUser;
    const long long total = deltaKernel + deltaUser;
    metrics.usagePercent = clampPercent(percent(static_cast<double>(busy), static_cast<double>(total)));
    return metrics;
}

MemoryMetrics WindowsMonitor::getMemory()
{
    MemoryMetrics metrics;
    MEMORYSTATUSEX status{};
    status.dwLength = sizeof(status);
    if (!::GlobalMemoryStatusEx(&status)) {
        return metrics;
    }
    metrics.usagePercent = static_cast<double>(status.dwMemoryLoad);
    metrics.totalMiB = static_cast<long long>(status.ullTotalPhys / (1024 * 1024));
    metrics.freeMiB = static_cast<long long>(status.ullAvailPhys / (1024 * 1024));
    metrics.usedMiB = metrics.totalMiB - metrics.freeMiB;
    return metrics;
}

GPUMetrics WindowsMonitor::getGPU()
{
    GPUMetrics metrics;
    if (!dxgiFactory_) {
        return metrics;
    }
    IDXGIAdapter1* adapter = nullptr;
    if (FAILED(reinterpret_cast<IDXGIFactory1*>(dxgiFactory_)->EnumAdapters1(0, &adapter))) {
        return metrics;
    }
    DXGI_ADAPTER_DESC1 desc{};
    if (SUCCEEDED(adapter->GetDesc1(&desc))) {
        char name[256] = {};
        ::WideCharToMultiByte(CP_UTF8, 0, desc.Description, -1, name, sizeof(name) - 1, nullptr, nullptr);
        metrics.name = name;
        metrics.vramTotalMiB = static_cast<double>(desc.DedicatedVideoMemory) / (1024.0 * 1024.0);
    }
    adapter->Release();
    return metrics;
}

BatteryMetrics WindowsMonitor::getBattery()
{
    BatteryMetrics metrics;
    SYSTEM_POWER_STATUS status{};
    if (!::GetSystemPowerStatus(&status) || status.BatteryLifePercent == 255) {
        return metrics;
    }
    metrics.percent = static_cast<double>(status.BatteryLifePercent);
    metrics.status = status.ACLineStatus == 1 ? "Charging" : "Discharging";
    return metrics;
}

} // namespace iso_kernos

#endif
