# Running xnos

This guide is focused only on setup, building, and running `xnos` across Linux, Windows, and macOS.

## Requirements

`xnos` is a C++17 project with no third-party runtime dependencies.

You need:

- CMake 3.16 or newer
- A C++17 compiler
- A terminal or shell
- Git, if cloning the repository

Recommended optional tool:

- Ninja, for faster CMake builds

## Linux Setup

### Ubuntu or Debian

```bash
sudo apt update
sudo apt install -y build-essential cmake ninja-build
```

### Fedora

```bash
sudo dnf install -y gcc-c++ cmake ninja-build
```

### Arch Linux

```bash
sudo pacman -S --needed base-devel cmake ninja
```

### Build

```bash
cmake -B build -G Ninja -DBUILD_TESTS=ON
cmake --build build --parallel
```

If Ninja is not installed:

```bash
cmake -B build -DBUILD_TESTS=ON
cmake --build build --parallel
```

### Run

```bash
./build/iso-kernos
```

Useful modes:

```bash
./build/iso-kernos --compact
./build/iso-kernos --mode detailed
./build/iso-kernos --json
./build/iso-kernos --duration 10
```

### Test

```bash
ctest --test-dir build --output-on-failure
```

## Windows Setup

Use either MinGW-w64 or Microsoft Visual Studio Build Tools.

### Option 1: MinGW-w64

Install:

- CMake
- Ninja
- MinGW-w64 with `g++`

Make sure `cmake`, `ninja`, and `g++` are available in PowerShell:

```powershell
cmake --version
ninja --version
g++ --version
```

Build:

```powershell
cmake -B build -G Ninja -DBUILD_TESTS=ON
cmake --build build --parallel
```

Run:

```powershell
.\build\iso-kernos.exe
```

Test:

```powershell
ctest --test-dir build --output-on-failure
```

### Option 2: Visual Studio Build Tools

Install:

- Visual Studio Build Tools 2022
- Desktop development with C++
- CMake tools for Windows, or standalone CMake

Open a Developer PowerShell or Developer Command Prompt.

Configure:

```powershell
cmake -B build -DBUILD_TESTS=ON
```

Build:

```powershell
cmake --build build --config Release --parallel
```

Run:

```powershell
.\build\Release\iso-kernos.exe
```

Test:

```powershell
ctest --test-dir build -C Release --output-on-failure
```

## macOS Setup

Install Xcode Command Line Tools:

```bash
xcode-select --install
```

Install CMake and Ninja with Homebrew:

```bash
brew install cmake ninja
```

Build:

```bash
cmake -B build -G Ninja -DBUILD_TESTS=ON
cmake --build build --parallel
```

Run:

```bash
./build/iso-kernos
```

Test:

```bash
ctest --test-dir build --output-on-failure
```

## Common Run Modes

Dashboard mode is the default:

```bash
iso-kernos
```

Compact single-line output:

```bash
iso-kernos --compact
```

Detailed output:

```bash
iso-kernos --mode detailed
```

JSON output for scripts:

```bash
iso-kernos --json
```

Run for a fixed number of seconds:

```bash
iso-kernos --duration 30
```

Collect once and exit:

```bash
iso-kernos --test-mode --mode compact
```

Use a custom refresh rate:

```bash
iso-kernos --refresh 500
```

Write JSON logs:

```bash
iso-kernos --log metrics.jsonl
```

Use a config file:

```bash
iso-kernos --config config/default_config.ini
```

## Configuration

The default config template is:

```text
config/default_config.ini
```

Example:

```ini
[display]
refresh_rate_ms = 1000
color_enabled = true

[alerts]
cpu_threshold = 90
memory_threshold = 85
```

Command-line options override the defaults used by the program.

## Platform Notes

### Linux

Linux metrics use native files such as:

- `/proc/stat`
- `/proc/loadavg`
- `/proc/meminfo`
- `/sys/class/drm`
- `/sys/class/power_supply`

GPU support depends on what the driver exposes. AMD and Intel GPUs may expose DRM sysfs metrics. NVIDIA can be detected through `nvidia-smi` if it is installed.

### Windows

Windows metrics use native APIs:

- `GetSystemTimes`
- `GlobalMemoryStatusEx`
- DXGI
- `GetSystemPowerStatus`

GPU utilization may be unavailable depending on driver/API support. Missing values show as `N/A`.

### macOS

The macOS backend is currently scaffolded. The project builds, but detailed native metrics still need implementation through Mach, `sysctl`, and IOKit.

## Troubleshooting

### CMake Cannot Find a Compiler

Install a C++ compiler and make sure it is available in your shell.

Linux:

```bash
g++ --version
```

Windows MinGW:

```powershell
g++ --version
```

Windows Visual Studio:

Use a Developer PowerShell or Developer Command Prompt.

macOS:

```bash
clang++ --version
```

### Ninja Not Found

Either install Ninja or omit `-G Ninja`:

```bash
cmake -B build -DBUILD_TESTS=ON
```

### Executable Path Is Different

Single-config generators usually place the executable here:

```text
build/iso-kernos
build/iso-kernos.exe
```

Visual Studio and other multi-config generators usually place it here:

```text
build/Release/iso-kernos.exe
build/Debug/iso-kernos.exe
```

### Metrics Show N/A

`N/A` means the metric is not exposed on that platform, driver, or machine. The program should continue running normally.

