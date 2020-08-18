# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(events STATIC)

include(${MBED_CONFIG_PATH}/mbed_config.cmake)

include(${MBED_ROOT}/cmake/core.cmake)
mbed_set_cpu_core_options(events ${MBED_TOOLCHAIN})

include(${MBED_ROOT}/cmake/toolchain.cmake)
mbed_set_toolchain_options(events)

include(${MBED_ROOT}/cmake/profile.cmake)
mbed_set_profile_options(events ${MBED_TOOLCHAIN})

target_include_directories(events
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include/events
        ${CMAKE_CURRENT_SOURCE_DIR}/include/events/internal
)

target_sources(events
    PRIVATE
        source/EventQueue.cpp
        source/equeue.c
        source/equeue_mbed.cpp
        source/equeue_posix.c
        source/mbed_shared_queues.cpp
)

target_link_libraries(events 
    PUBLIC
        platform
        rtos
        drivers
)

