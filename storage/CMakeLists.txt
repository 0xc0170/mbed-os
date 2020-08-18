# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(storage STATIC)

include(${MBED_CONFIG_PATH}/mbed_config.cmake)

mbed_set_cpu_core_options(storage ${MBED_TOOLCHAIN})
mbed_set_toolchain_options(storage)
mbed_set_language_standard(storage)
mbed_set_profile_options(storage ${MBED_TOOLCHAIN})

include(${MBED_ROOT}/cmake/util.cmake)

add_subdirectory(blockdevice)
add_subdirectory(filesystem)
add_subdirectory(kvstore)
add_subdirectory(platform)

target_include_directories(storage
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(mbed-os 
    PUBLIC
        platform
        rtos
        targets
)
