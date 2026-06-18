cmake_minimum_required(VERSION 3.16)

project(iso-kernos VERSION 1.0.0 LANGUAGES CXX)

option(BUILD_TESTS "Build iso-kernos tests" ON)
option(ENABLE_WARNINGS "Enable compiler warnings" ON)
option(WARNINGS_AS_ERRORS "Treat warnings as errors" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(ISO_KERNOS_CORE_SOURCES
    src/core/Config.cpp
    src/core/MetricCollector.cpp
    src/core/MonitorFactory.cpp
    src/display/Formatter.cpp
    src/display/TerminalUI.cpp
    src/alerts/AlertManager.cpp
    src/alerts/Logger.cpp
    src/utils/FileReader.cpp
    src/utils/StringUtils.cpp
    src/utils/TimeUtils.cpp
    src/utils/MathUtils.cpp)

if(WIN32)
    list(APPEND ISO_KERNOS_CORE_SOURCES src/platform/windows/WindowsMonitor.cpp)
elseif(APPLE)
    list(APPEND ISO_KERNOS_CORE_SOURCES src/platform/macos/MacOSMonitor.cpp)
elseif(UNIX)
    list(APPEND ISO_KERNOS_CORE_SOURCES
        src/platform/linux/LinuxMonitor.cpp
        src/platform/linux/ProcParser.cpp
        src/platform/linux/SysParser.cpp)
endif()

add_library(iso_kernos_core STATIC ${ISO_KERNOS_CORE_SOURCES})
target_include_directories(iso_kernos_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

if(WIN32)
    target_link_libraries(iso_kernos_core PRIVATE dxgi)
elseif(APPLE)
    find_library(IOKIT_FRAMEWORK IOKit)
    find_library(COREFOUNDATION_FRAMEWORK CoreFoundation)
    if(IOKIT_FRAMEWORK AND COREFOUNDATION_FRAMEWORK)
        target_link_libraries(iso_kernos_core PRIVATE ${IOKIT_FRAMEWORK} ${COREFOUNDATION_FRAMEWORK})
    endif()
endif()

if(ENABLE_WARNINGS)
    if(MSVC)
        target_compile_options(iso_kernos_core PRIVATE /W4)
    else()
        target_compile_options(iso_kernos_core PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endif()

if(WARNINGS_AS_ERRORS)
    if(MSVC)
        target_compile_options(iso_kernos_core PRIVATE /WX)
    else()
        target_compile_options(iso_kernos_core PRIVATE -Werror)
    endif()
endif()

add_executable(iso-kernos src/main.cpp)
target_link_libraries(iso-kernos PRIVATE iso_kernos_core)

if(BUILD_TESTS)
    enable_testing()
    add_executable(iso-kernos-tests
        tests/unit/test_formatters.cpp
        tests/unit/test_metric_collector.cpp)
    target_link_libraries(iso-kernos-tests PRIVATE iso_kernos_core)
    add_test(NAME iso-kernos-tests COMMAND iso-kernos-tests)

    add_executable(iso-kernos-e2e tests/integration/test_monitor_e2e.cpp)
    target_link_libraries(iso-kernos-e2e PRIVATE iso_kernos_core)
    add_test(NAME iso-kernos-e2e COMMAND iso-kernos-e2e)
endif()
