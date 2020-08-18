# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

#mbed_add_cmake_directory_if_labels("TARGET")

add_subdirectory(TARGET_MCUXpresso_MCUS)
add_subdirectory(TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F)
add_subdirectory(TARGET_MCUXpresso_MCUS/TARGET_MCU_K64F/TARGET_FRDM)

target_include_directories(targets
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(targets
    PRIVATE
        USBPhy_Kinetis.cpp
)
