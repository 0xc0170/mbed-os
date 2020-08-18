# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(rtos STATIC)

include(${MBED_CONFIG_PATH}/mbed_config.cmake)

mbed_set_cpu_core_options(rtos ${MBED_TOOLCHAIN})
mbed_set_toolchain_options(rtos)
mbed_set_language_standard(rtos)
mbed_set_profile_options(rtos ${MBED_TOOLCHAIN})

add_subdirectory(source/TARGET_CORTEX)

target_include_directories(rtos
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/rtos
        ${CMAKE_CURRENT_SOURCE_DIR}/include/rtos/internal
        ${CMAKE_CURRENT_SOURCE_DIR}/source
)

target_sources(rtos
    PRIVATE
        source/ConditionVariable.cpp
        source/EventFlags.cpp
        source/Kernel.cpp
        source/Mutex.cpp
        source/Semaphore.cpp
        source/ThisThread.cpp
        source/Thread.cpp
)

target_link_libraries(rtos 
    PUBLIC
        targets
        platform
)
