# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(targets STATIC)

include(${MBED_CONFIG_PATH}/mbed_config.cmake)

mbed_set_cpu_core_options(targets ${MBED_TOOLCHAIN})
mbed_set_toolchain_options(targets)
mbed_set_language_standard(targets)
mbed_set_profile_options(targets ${MBED_TOOLCHAIN})

#mbed_add_cmake_directory_if_labels("TARGET")

target_compile_definitions(targets
    PUBLIC
        ${MBED_TARGET_DEFINITIONS}
        ${MBED_CONFIG_DEFINITIONS}
)

include(${MBED_ROOT}/cmake/util.cmake)

#mbed_add_cmake_directory_if_labels("TARGET")
add_subdirectory(TARGET_Freescale)

target_link_libraries(targets 
    PUBLIC
        hal
        drivers
        platform
        rtos
        cmsis
)
