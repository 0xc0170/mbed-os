# SPDX-License-Identifier: Apache-2.0

add_library(cmsis STATIC)

include(${MBED_CONFIG_PATH}/mbed_config.cmake)

#include(${MBED_ROOT}/cmake/toolchain.cmake)
mbed_set_toolchain_options(cmsis)

#include(${MBED_ROOT}/cmake/profile.cmake)
mbed_set_profile_options(cmsis ${MBED_TOOLCHAIN})

#mbed_add_cmake_directory_if_labels("TARGET")

add_subdirectory(TARGET_CORTEX_M)

target_sources(cmsis
    PRIVATE
        RTE_Components.h
)

target_include_directories(cmsis
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
