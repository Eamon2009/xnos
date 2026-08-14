# iso-kernos: Production-Level Development Prompt

## Project Overview
**iso-kernos** is a cross-platform hardware monitoring tool written in C++ that provides real-time system metrics (CPU, RAM, GPU, Disk, Network) without external dependencies. The project targets Linux, Windows, and macOS platforms with native OS APIs.

---

## Project Goals

1. **Zero External Dependencies**: Use only standard C++17/20 and OS-native APIs
2. **Cross-Platform Consistency**: Uniform API across all platforms with platform-specific implementations
3. **Production-Grade Quality**: Clean architecture, comprehensive testing, CI/CD integration
4. **Performance**: Minimal overhead, efficient polling, no memory leaks
5. **User-Friendly**: Clear terminal UI, configurable, scriptable

---

##  Directory Structure

```
iso-kernos/
├── .github/
│   └── workflows/
│       ├── ci.yml                    # Main CI/CD pipeline
│       ├── release.yml               # Release automation
│       └── codeql.yml                # Security analysis
├── src/
│   ├── core/
│   │   ├── IMonitor.hpp              # Abstract monitor interface
│   │   ├── MonitorFactory.hpp/cpp    # Factory for platform-specific monitors
│   │   ├── MetricCollector.hpp/cpp   # Metric aggregation logic
│   │   └── Config.hpp/cpp            # Configuration management
│   ├── platform/
│   │   ├── linux/
│   │   │   ├── LinuxMonitor.hpp/cpp  # Linux implementation
│   │   │   ├── ProcParser.hpp/cpp    # /proc filesystem parser
│   │   │   └── SysParser.hpp/cpp     # /sys filesystem parser
│   │   ├── windows/
│   │   │   ├── WindowsMonitor.hpp/cpp # Windows implementation
│   │   │   ├── PDHWrapper.hpp/cpp     # Performance Data Helper wrapper
│   │   │   └── WMIQuery.hpp/cpp       # WMI query utilities
│   │   └── macos/
│   │       ├── MacOSMonitor.hpp/cpp   # macOS implementation
│   │       ├── SysctlWrapper.hpp/cpp  # sysctl API wrapper
│   │       └── IOKitWrapper.hpp/cpp   # IOKit framework wrapper
│   ├── utils/
│   │   ├── StringUtils.hpp/cpp        # String parsing helpers
│   │   ├── FileReader.hpp/cpp         # Efficient file reading
│   │   ├── TimeUtils.hpp/cpp          # Timestamp and duration utils
│   │   └── MathUtils.hpp/cpp          # Average, percentage calculations
│   ├── display/
│   │   ├── TerminalUI.hpp/cpp         # Terminal rendering
│   │   ├── Formatter.hpp/cpp          # Data formatting
│   │   ├── ColorScheme.hpp/cpp        # ANSI color management
│   │   └── Layout.hpp/cpp             # UI layout manager
│   ├── alerts/
│   │   ├── AlertManager.hpp/cpp       # Threshold monitoring
│   │   └── Logger.hpp/cpp             # Event logging
│   └── main.cpp                       # Entry point
├── tests/
│   ├── unit/
│   │   ├── test_proc_parser.cpp
│   │   ├── test_metric_collector.cpp
│   │   └── test_formatters.cpp
│   ├── integration/
│   │   └── test_monitor_e2e.cpp
│   └── mocks/
│       └── MockMonitor.hpp/cpp
├── scripts/
│   ├── build.sh                       # Cross-platform build script
│   ├── install.sh                     # Installation script
│   └── package.sh                     # Packaging script
├── docs/
│   ├── ARCHITECTURE.md                # System design documentation
│   ├── API.md                         # API reference
│   ├── PLATFORM_NOTES.md              # Platform-specific details
│   └── CONTRIBUTING.md                # Contribution guidelines
├── config/
│   └── default_config.ini             # Default configuration template
├── CMakeLists.txt                     # Root CMake configuration
├── .clang-format                      # Code formatting rules
├── .gitignore
├── .gitattributes
├── LICENSE
└── README.md
```

---

## Core Features to Implement

### 1. CPU Monitoring
**Linux**: `/proc/stat`, `/proc/cpuinfo`, `/sys/devices/system/cpu/`
- [x] Overall CPU utilization
- [ ] Per-core utilization
- [ ] CPU frequency (current, min, max)
- [ ] CPU temperature (`/sys/class/thermal/`)
- [ ] Load average (1min, 5min, 15min)
- [ ] Context switches and interrupts
- [ ] Process count (running, sleeping, zombie)

**Windows**: PDH API, WMI
- [ ] `\Processor(_Total)\% Processor Time`
- [ ] Per-core metrics via WMI `Win32_PerfFormattedData_PerfOS_Processor`
- [ ] Temperature via WMI `MSAcpi_ThermalZoneTemperature`

**macOS**: `host_processor_info()`, `sysctl`
- [ ] `host_processor_info(HOST_CPU_LOAD_INFO)`
- [ ] `sysctl kern.cp_time` for CPU ticks
- [ ] SMC (System Management Controller) for temperature

### 2. Memory Monitoring
**Linux**: `/proc/meminfo`, `/proc/vmstat`
- [x] Total/Used/Free RAM
- [ ] Buffer/Cache breakdown
- [ ] Swap usage
- [ ] Memory pressure (PSI)
- [ ] Huge pages info
- [ ] Per-process RSS/VSZ

**Windows**: `GlobalMemoryStatusEx()`, PDH
- [ ] Physical/Virtual memory stats
- [ ] Committed memory
- [ ] Page file usage

**macOS**: `host_statistics64()`, `vm_stat`
- [ ] Wired/Active/Inactive/Free memory
- [ ] Swap usage

### 3. GPU Monitoring
**Linux**: `/sys/class/drm/`, `/sys/kernel/debug/dri/`
- [x] Basic GPU info (vendor, model)
- [ ] GPU utilization
- [ ] VRAM usage
- [ ] GPU temperature
- [ ] Clock speeds
- [ ] Power draw

**Windows**: DXGI API, WMI
- [ ] GPU utilization via `Win32_PerfFormattedData_GPUPerformanceCounters`
- [ ] VRAM via `IDXGIAdapter::QueryVideoMemoryInfo()`

**macOS**: IOKit framework
- [ ] GPU info via `IOServiceMatching("IOAccelerator")`
- [ ] Metal framework queries

### 4. Disk I/O Monitoring
**Linux**: `/proc/diskstats`, `/sys/block/`
- [ ] Read/write bytes per second
- [ ] IOPS (read/write operations)
- [ ] Disk utilization percentage
- [ ] Queue depth
- [ ] Per-partition usage

**Windows**: PDH `\LogicalDisk\` counters
- [ ] Disk read/write bytes/sec
- [ ] Disk queue length

**macOS**: `statfs()`, IOKit
- [ ] Disk I/O statistics

### 5. Network Monitoring
**Linux**: `/proc/net/dev`, `/sys/class/net/`
- [ ] Bytes sent/received per interface
- [ ] Packets sent/received
- [ ] Errors and drops
- [ ] Interface status (up/down)
- [ ] Link speed

**Windows**: `GetIfTable2()`, `GetIfEntry2()`
- [ ] Network interface stats

**macOS**: `getifaddrs()`, `sysctl`
- [ ] Network interface statistics

### 6. Battery Monitoring (Laptops)
**Linux**: `/sys/class/power_supply/`
- [ ] Battery percentage
- [ ] Charging status
- [ ] Time remaining
- [ ] Power consumption

**Windows**: `GetSystemPowerStatus()`, WMI
- [ ] Battery info via `Win32_Battery`

**macOS**: IOKit `IOPowerSources`
- [ ] Battery statistics

### 7. System Information
- [ ] OS name and version
- [ ] Kernel version
- [ ] Hostname
- [ ] System uptime
- [ ] Boot time
- [ ] Logged-in users

### 8. Process Monitoring
- [ ] Top N processes by CPU usage
- [ ] Top N processes by memory usage
- [ ] Process tree visualization
- [ ] Process state (running, sleeping, zombie)

---

## Display Features

### Terminal UI Modes
1. **Dashboard Mode** (Default)
   - Full-screen TUI with sections for each metric
   - Real-time updates
   - Color-coded alerts (green/yellow/red)
   - Progress bars for utilization

2. **Compact Mode** (`--compact`)
   - Single-line output suitable for tmux/status bars
   - Example: `CPU: 49% | RAM: 86% | GPU: N/A`

3. **Detailed Mode** (`--detailed`)
   - Verbose output with all available metrics
   - Per-core, per-disk, per-interface breakdown

4. **JSON Mode** (`--json`)
   - Machine-readable output for scripting
   - Single JSON object per update

5. **Log Mode** (`--log <file>`)
   - Continuous logging to file
   - CSV or JSON format

### Visual Enhancements
- [ ] ASCII sparklines for historical trends
- [ ] Color-coded thresholds (e.g., >90% = red)
- [ ] Unicode box-drawing characters for clean borders
- [ ] Smooth scrolling for process lists
- [ ] Percentage bars: `[████████░░] 80%`

---

## ⚙️ Configuration System

### Configuration File (`~/.config/iso-kernos/config.ini`)
```ini
[display]
refresh_rate_ms = 1000
color_enabled = true
theme = default

[alerts]
cpu_threshold = 90
memory_threshold = 85
disk_threshold = 95
alert_sound = true

[metrics]
show_cpu = true
show_memory = true
show_gpu = true
show_disk = true
show_network = true
show_battery = true

[advanced]
history_samples = 60
log_file = /var/log/iso-kernos.log
```

### Command-Line Arguments
```
Usage: iso-kernos [OPTIONS]

Options:
  -h, --help              Show this help message
  -v, --version           Show version information
  -r, --refresh <ms>      Refresh rate in milliseconds (default: 1000)
  -m, --mode <mode>       Display mode: dashboard|compact|detailed|json
  -c, --config <file>     Custom config file path
  -l, --log <file>        Log output to file
  --no-color              Disable colored output
  --cpu-only              Show only CPU metrics
  --mem-only              Show only memory metrics
  --threshold-cpu <n>     CPU alert threshold (0-100)
  --threshold-mem <n>     Memory alert threshold (0-100)
```

---

## Patterns

### 1. Abstract Factory Pattern
```cpp
class IMonitor {
public:
    virtual ~IMonitor() = default;
    virtual CPUMetrics getCPU() = 0;
    virtual MemoryMetrics getMemory() = 0;
    virtual GPUMetrics getGPU() = 0;
    // ... other metric getters
};

class MonitorFactory {
public:
    static std::unique_ptr<IMonitor> create() {
#ifdef __linux__
        return std::make_unique<LinuxMonitor>();
#elif _WIN32
        return std::make_unique<WindowsMonitor>();
#elif __APPLE__
        return std::make_unique<MacOSMonitor>();
#endif
    }
};
```

### 2. Observer Pattern (Alert System)
```cpp
class IAlertListener {
public:
    virtual void onThresholdExceeded(const std::string& metric, double value) = 0;
};

class AlertManager {
    std::vector<IAlertListener*> listeners;
    void notify(const std::string& metric, double value);
};
```

### 3. Strategy Pattern (Display Strategies)
```cpp
class IDisplayStrategy {
public:
    virtual void render(const SystemMetrics& metrics) = 0;
};

class DashboardDisplay : public IDisplayStrategy { /* ... */ };
class CompactDisplay : public IDisplayStrategy { /* ... */ };
class JSONDisplay : public IDisplayStrategy { /* ... */ };
```

---

## Testing Strategy

### Unit Tests
- **Platform Parsers**: Mock file content, verify parsing logic
- **Metric Calculators**: Test percentage calculations, averages
- **Formatters**: Test output formatting with edge cases

### Integration Tests
- **End-to-End**: Create monitor → collect metrics → verify data types
- **Cross-Module**: Test factory + concrete implementations

### Platform Testing
- Use GitHub Actions matrix to test on Ubuntu, Windows, macOS
- Mock system files for deterministic tests

### Test Framework
- Use built-in C++ testing (no external deps) or minimal header-only libs
- Example: Simple assertion macros

```cpp
#define ASSERT_EQ(a, b) if ((a) != (b)) { throw std::runtime_error("Assertion failed"); }
```

---

## GitHub Actions Workflows

### 1. Main CI/CD Pipeline (`.github/workflows/ci.yml`)

```yaml
name: CI/CD Pipeline

on:
  push:
    branches: [ main, develop ]
  pull_request:
    branches: [ main, develop ]

jobs:
  # Job 1: Build and Test on Multiple Platforms
  build-and-test:
    name: Build & Test (${{ matrix.os }})
    runs-on: ${{ matrix.os }}
    strategy:
      fail-fast: false
      matrix:
        os: [ubuntu-22.04, ubuntu-20.04, windows-2022, windows-2019, macos-13, macos-12]
        build_type: [Release, Debug]
        include:
          - os: ubuntu-22.04
            compiler: gcc
            version: 11
          - os: ubuntu-20.04
            compiler: gcc
            version: 9
          - os: windows-2022
            compiler: msvc
          - os: windows-2019
            compiler: msvc
          - os: macos-13
            compiler: clang
          - os: macos-12
            compiler: clang

    steps:
      - name: Checkout Repository
        uses: actions/checkout@v4
        with:
          fetch-depth: 0

      - name: Setup C++ (Linux)
        if: runner.os == 'Linux'
        run: |
          sudo apt-get update
          sudo apt-get install -y build-essential cmake ninja-build

      - name: Setup C++ (Windows)
        if: runner.os == 'Windows'
        uses: microsoft/setup-msbuild@v1.1

      - name: Setup C++ (macOS)
        if: runner.os == 'macOS'
        run: |
          brew install cmake ninja

      - name: Configure CMake
        run: |
          cmake -B build -G Ninja \
            -DCMAKE_BUILD_TYPE=${{ matrix.build_type }} \
            -DBUILD_TESTS=ON \
            -DENABLE_WARNINGS=ON \
            -DWARNINGS_AS_ERRORS=ON

      - name: Build
        run: cmake --build build --config ${{ matrix.build_type }} --parallel

      - name: Run Unit Tests
        run: |
          cd build
          ctest --output-on-failure --build-config ${{ matrix.build_type }}

      - name: Run Integration Tests
        run: |
          cd build
          ./bin/iso-kernos --test-mode --duration 5

      - name: Upload Build Artifacts
        if: matrix.build_type == 'Release'
        uses: actions/upload-artifact@v4
        with:
          name: iso-kernos-${{ matrix.os }}
          path: |
            build/bin/iso-kernos*
            build/bin/*.exe
          retention-days: 7

  # Job 2: Code Quality Checks
  code-quality:
    name: Code Quality
    runs-on: ubuntu-latest
    steps:
      - name: Checkout Repository
        uses: actions/checkout@v4

      - name: Install Dependencies
        run: |
          sudo apt-get update
          sudo apt-get install -y clang-format clang-tidy cppcheck

      - name: Check Formatting (clang-format)
        run: |
          find src tests -name '*.cpp' -o -name '*.hpp' | \
          xargs clang-format --dry-run --Werror

      - name: Static Analysis (clang-tidy)
        run: |
          cmake -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
          find src -name '*.cpp' | \
          xargs clang-tidy -p build --warnings-as-errors='*'

      - name: Static Analysis (cppcheck)
        run: |
          cppcheck --enable=all --error-exitcode=1 \
            --suppress=missingIncludeSystem \
            --inline-suppr \
            src/

  # Job 3: Memory Leak Detection (Valgrind on Linux)
  memory-check:
    name: Memory Leak Check
    runs-on: ubuntu-latest
    steps:
      - name: Checkout Repository
        uses: actions/checkout@v4

      - name: Install Valgrind
        run: sudo apt-get install -y valgrind cmake ninja-build

      - name: Build with Debug Symbols
        run: |
          cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
          cmake --build build

      - name: Run Valgrind
        run: |
          valgrind --leak-check=full --error-exitcode=1 \
            ./build/bin/iso-kernos --duration 10

  # Job 4: Cross-Compilation Test
  cross-compile:
    name: Cross-Compile (ARM64)
    runs-on: ubuntu-latest
    steps:
      - name: Checkout Repository
        uses: actions/checkout@v4

      - name: Setup Cross-Compilation Toolchain
        run: |
          sudo apt-get update
          sudo apt-get install -y gcc-aarch64-linux-gnu g++-aarch64-linux-gnu

      - name: Build for ARM64
        run: |
          cmake -B build \
            -DCMAKE_SYSTEM_NAME=Linux \
            -DCMAKE_SYSTEM_PROCESSOR=aarch64 \
            -DCMAKE_C_COMPILER=aarch64-linux-gnu-gcc \
            -DCMAKE_CXX_COMPILER=aarch64-linux-gnu-g++
          cmake --build build

  # Job 5: Performance Benchmarks
  benchmark:
    name: Performance Benchmark
    runs-on: ubuntu-latest
    steps:
      - name: Checkout Repository
        uses: actions/checkout@v4

      - name: Build Release
        run: |
          cmake -B build -DCMAKE_BUILD_TYPE=Release
          cmake --build build

      - name: Run Benchmark
        run: |
          ./build/bin/iso-kernos --benchmark --iterations 1000 > benchmark.txt

      - name: Upload Benchmark Results
        uses: actions/upload-artifact@v4
        with:
          name: benchmark-results
          path: benchmark.txt
```

### 2. Release Workflow (`.github/workflows/release.yml`)

```yaml
name: Release

on:
  push:
    tags:
      - 'v*.*.*'

jobs:
  create-release:
    name: Create Release
    runs-on: ubuntu-latest
    outputs:
      upload_url: ${{ steps.create_release.outputs.upload_url }}
    steps:
      - name: Create Release
        id: create_release
        uses: actions/create-release@v1
        env:
          GITHUB_TOKEN: ${{ secrets.GITHUB_TOKEN }}
        with:
          tag_name: ${{ github.ref }}
          release_name: Release ${{ github.ref }}
          draft: false
          prerelease: false

  build-release:
    name: Build Release (${{ matrix.os }})
    needs: create-release
    runs-on: ${{ matrix.os }}
    strategy:
      matrix:
        include:
          - os: ubuntu-22.04
            artifact_name: iso-kernos
            asset_name: iso-kernos-linux-amd64
          - os: windows-2022
            artifact_name: iso-kernos.exe
            asset_name: iso-kernos-windows-amd64.exe
          - os: macos-13
            artifact_name: iso-kernos
            asset_name: iso-kernos-macos-amd64

    steps:
      - name: Checkout Repository
        uses: actions/checkout@v4

      - name: Build Release Binary
        run: |
          cmake -B build -DCMAKE_BUILD_TYPE=Release
          cmake --build build --config Release

      - name: Package Binary (Linux/macOS)
        if: runner.os != 'Windows'
        run: |
          cd build/bin
          tar -czf ${{ matrix.asset_name }}.tar.gz ${{ matrix.artifact_name }}

      - name: Package Binary (Windows)
        if: runner.os == 'Windows'
        run: |
          cd build/bin
          7z a ${{ matrix.asset_name }}.zip ${{ matrix.artifact_name }}

      - name: Upload Release Asset
        uses: actions/upload-release-asset@v1
        env:
          GITHUB_TOKEN: ${{ secrets.GITHUB_TOKEN }}
        with:
          upload_url: ${{ needs.create-release.outputs.upload_url }}
          asset_path: ./build/bin/${{ matrix.asset_name }}.*
          asset_name: ${{ matrix.asset_name }}.*
          asset_content_type: application/octet-stream

  # Build packages for Linux distributions
  build-packages:
    name: Build Packages
    needs: create-release
    runs-on: ubuntu-latest
    steps:
      - name: Checkout Repository
        uses: actions/checkout@v4

      - name: Install Packaging Tools
        run: |
          sudo apt-get update
          sudo apt-get install -y rpm dpkg-dev

      - name: Build DEB Package
        run: |
          cmake -B build -DCMAKE_BUILD_TYPE=Release
          cmake --build build
          cd build && cpack -G DEB

      - name: Build RPM Package
        run: |
          cd build && cpack -G RPM

      - name: Upload DEB Package
        uses: actions/upload-release-asset@v1
        env:
          GITHUB_TOKEN: ${{ secrets.GITHUB_TOKEN }}
        with:
          upload_url: ${{ needs.create-release.outputs.upload_url }}
          asset_path: ./build/iso-kernos*.deb
          asset_name: iso-kernos.deb
          asset_content_type: application/vnd.debian.binary-package

      - name: Upload RPM Package
        uses: actions/upload-release-asset@v1
        env:
          GITHUB_TOKEN: ${{ secrets.GITHUB_TOKEN }}
        with:
          upload_url: ${{ needs.create-release.outputs.upload_url }}
          asset_path: ./build/iso-kernos*.rpm
          asset_name: iso-kernos.rpm
          asset_content_type: application/x-rpm
```

### 3. Security Scanning (`.github/workflows/codeql.yml`)

```yaml
name: CodeQL Security Analysis

on:
  push:
    branches: [ main ]
  pull_request:
    branches: [ main ]
  schedule:
    - cron: '0 0 * * 0'  # Weekly on Sunday

jobs:
  analyze:
    name: Analyze Code
    runs-on: ubuntu-latest
    permissions:
      security-events: write
      actions: read
      contents: read

    steps:
      - name: Checkout Repository
        uses: actions/checkout@v4

      - name: Initialize CodeQL
        uses: github/codeql-action/init@v3
        with:
          languages: cpp

      - name: Build Project
        run: |
          cmake -B build
          cmake --build build

      - name: Perform CodeQL Analysis
        uses: github/codeql-action/analyze@v3
```

### 4. Documentation Build (`.github/workflows/docs.yml`)

```yaml
name: Documentation

on:
  push:
    branches: [ main ]
    paths:
      - 'docs/**'
      - 'README.md'

jobs:
  deploy-docs:
    name: Deploy Documentation
    runs-on: ubuntu-latest
    steps:
      - name: Checkout Repository
        uses: actions/checkout@v4

      - name: Build Documentation
        run: |
          # If using Doxygen
          sudo apt-get install -y doxygen graphviz
          doxygen Doxyfile

      - name: Deploy to GitHub Pages
        uses: peaceiris/actions-gh-pages@v3
        with:
          github_token: ${{ secrets.GITHUB_TOKEN }}
          publish_dir: ./docs/html

---

## Performance Optimization Guidelines

1. **Minimize File I/O**: Cache static info (CPU model, total RAM)
2. **Use Buffered Reads**: Read entire files into memory for parsing
3. **Avoid String Allocations**: Use `string_view` where possible
4. **Efficient Parsing**: Use `strtol`, `strtod` instead of streams
5. **Non-blocking Updates**: Async collection of slow metrics (GPU temp)
6. **Smart Refresh Rates**: Poll fast-changing metrics more frequently

---

## Metrics to Track in CI/CD

- **Build Time**: Per platform and configuration
- **Binary Size**: Track size increases over releases
- **Memory Usage**: Peak RSS during 1-hour run
- **CPU Overhead**: Self-monitoring overhead percentage
- **Code Coverage**: Aim for >80% line coverage

---

## Security Considerations

1. **Input Validation**: Sanitize all file reads (prevent path traversal)
2. **Buffer Overflows**: Use `std::string`, `std::vector` (no raw arrays)
3. **Privilege Escalation**: Never require root/admin unless absolutely necessary
4. **Dependency Scanning**: Regular CodeQL and vulnerability checks
5. **Secure Defaults**: Don't log sensitive system info by default

---

## Documentation Requirements

### README.md
- Project description and screenshot
- Installation instructions (per platform)
- Quick start guide
- Feature list
- Command-line reference
- License and contributing links

### ARCHITECTURE.md
- High-level system design
- Component diagrams
- Platform abstraction layer explanation
- Data flow diagrams

### API.md
- Public API reference for `IMonitor`
- Metric struct definitions
- Example usage code

### PLATFORM_NOTES.md
- Linux-specific details (required kernel version, proc/sys dependencies)
- Windows-specific details (PDH counters, WMI namespaces)
- macOS-specific details (framework requirements, permissions)

### CONTRIBUTING.md
- Code style guide (clang-format config)
- Pull request process
- Testing requirements
- Commit message conventions

---

##  Phases

### Phase 1: Core Foundation 
- [ ] Project structure setup
- [ ] CMake configuration for all platforms
- [ ] Abstract `IMonitor` interface
- [ ] Factory pattern implementation
- [ ] Basic CI/CD pipeline

### Phase 2: Linux 
- [ ] CPU monitoring (`/proc/stat`, `/proc/cpuinfo`)
- [ ] Memory monitoring (`/proc/meminfo`)
- [ ] GPU monitoring (DRM subsystem)
- [ ] Basic terminal UI
- [ ] Unit tests for parsers

### Phase 3: Windows Implementation 
- [ ] PDH API integration
- [ ] WMI queries for detailed metrics
- [ ] GPU monitoring (DXGI)
- [ ] Cross-platform UI adaptation

### Phase 4: macOS Implementation
- [ ] Sysctl API integration
- [ ] IOKit framework usage
- [ ] Metal framework for GPU
- [ ] Code signing and notarization

### Phase 5: Advanced Features 
- [ ] Disk I/O monitoring
- [ ] Network monitoring
- [ ] Battery monitoring
- [ ] Alert system
- [ ] Configuration file support

### Phase 6: Polish & Release 
- [ ] Performance optimization
- [ ] Memory leak fixes
- [ ] Comprehensive testing
- [ ] Documentation completion
- [ ] Release packaging

---

## Success Criteria

1. **Cross-Platform**: Builds and runs on Ubuntu 20.04+, Windows 10+, macOS 12+
2. **Zero Dependencies**: No external libraries required
3. **Performance**: <1% CPU overhead, <50MB memory footprint
4. **Test Coverage**: >80% code coverage
5. **CI/CD**: All platforms green in GitHub Actions
6. **Documentation**: Complete API and user documentation
7. **Packaging**: Native installers for all platforms

---

## Reference Resources

### Linux
- [proc(5) man page](https://man7.org/linux/man-pages/man5/proc.5.html)
- [sysfs documentation](https://www.kernel.org/doc/html/latest/filesystems/sysfs.html)
- [DRM subsystem](https://www.kernel.org/doc/html/latest/gpu/index.html)

### Windows
- [PDH API Reference](https://docs.microsoft.com/en-us/windows/win32/perfctrs/performance-counters-portal)
- [WMI Reference](https://docs.microsoft.com/en-us/windows/win32/wmisdk/wmi-start-page)
- [DXGI Overview](https://docs.microsoft.com/en-us/windows/win32/direct3ddxgi/d3d10-graphics-programming-guide-dxgi)

### macOS
- [sysctl(3) man page](https://developer.apple.com/library/archive/documentation/System/Conceptual/ManPages_iPhoneOS/man3/sysctl.3.html)
- [IOKit Fundamentals](https://developer.apple.com/library/archive/documentation/DeviceDrivers/Conceptual/IOKitFundamentals/)
- [Metal Performance Shaders](https://developer.apple.com/documentation/metalperformanceshaders)

---

## Contributing

- Follow C++17 best practices
- Include unit tests (>80% coverage for new code)
- Pass all CI checks (formatting, static analysis, build)
- Update documentation
- Maintain cross-platform compatibility

---

## 📄 License

Apache-2.0 License 
