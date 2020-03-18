# SPDX-License-Identifier: Apache-2.0

add_library(cmsis STATIC)

# TODO: @mbed-os-tools how to get config in the cmsis lib
target_link_libraries(cmsis PUBLIC gen_config)

mbed_add_cmake_directory_if_labels("TARGET")

target_sources(cmsis PRIVATE RTE_Components.h)

target_include_directories(cmsis PUBLIC .)

