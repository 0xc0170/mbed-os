# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(targets STATIC)

include(${MBED_CONFIG_PATH}/mbed_config.cmake)

#mbed_add_cmake_directory_if_labels("TARGET")

# TODO: the above is including multiple
add_subdirectory(TARGET_Freescale)

target_link_libraries(targets 
    PUBLIC
        drivers
        platform
        hal
        rtos
)
