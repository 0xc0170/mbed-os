# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(platform STATIC)

include(${MBED_CONFIG_PATH}/mbed_config.cmake)

mbed_set_cpu_core_options(platform ${MBED_TOOLCHAIN})
mbed_set_toolchain_options(platform)
mbed_set_language_standard(platform)
mbed_set_profile_options(platform ${MBED_TOOLCHAIN})

add_subdirectory(cxxsupport)
add_subdirectory(source/TARGET_CORTEX_M)
add_subdirectory(source/minimal-printf)

target_include_directories(platform
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/platform
        ${CMAKE_CURRENT_SOURCE_DIR}/include/platform/internal
        ${CMAKE_CURRENT_SOURCE_DIR}/source
)

target_sources(platform
    PRIVATE
        source/ATCmdParser.cpp
        source/CThunkBase.cpp
        source/CriticalSectionLock.cpp
        source/DeepSleepLock.cpp
        source/FileBase.cpp
        source/FileHandle.cpp
        source/FilePath.cpp
        source/FileSystemHandle.cpp
        source/LocalFileSystem.cpp
        source/Stream.cpp
        source/SysTimer.cpp
        source/mbed_alloc_wrappers.cpp
        source/mbed_application.c
        source/mbed_assert.c
        source/mbed_atomic_impl.c
        source/mbed_board.c
        source/mbed_critical.c
        source/mbed_error.c
        source/mbed_error_hist.c
        source/mbed_interface.c
        source/mbed_mem_trace.cpp
        source/mbed_mktime.c
        source/mbed_mpu_mgmt.c
        source/mbed_os_timer.cpp
        source/mbed_poll.cpp
        source/mbed_power_mgmt.c
        source/mbed_retarget.cpp
        source/mbed_rtc_time.cpp
        source/mbed_sdk_boot.c
        source/mbed_semihost_api.c
        source/mbed_stats.c
        source/mbed_thread.cpp
        source/mbed_wait_api_no_rtos.c
)

target_link_libraries(platform 
    PUBLIC
        rtos
        hal
        targets
)
