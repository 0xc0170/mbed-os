# SPDX-License-Identifier: Apache-2.0

add_library(cmsis STATIC)

mbed_add_cmake_directory_if_labels("TARGET")

target_sources(cmsis
    PRIVATE
        RTE_Components.h
)

target_include_directories(cmsis
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
