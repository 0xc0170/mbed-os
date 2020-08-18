# SPDX-License-Identifier: Apache-2.0

add_library(cmsis STATIC)

include(${MBED_CONFIG_PATH}/mbed_config.cmake)

mbed_set_cpu_core_options(cmsis ${MBED_TOOLCHAIN})
mbed_set_toolchain_options(cmsis)
mbed_set_language_standard(cmsis)
mbed_set_profile_options(cmsis ${MBED_TOOLCHAIN})

include(${MBED_ROOT}/cmake/util.cmake)

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
