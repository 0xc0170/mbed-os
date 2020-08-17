# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(targets STATIC)

include(${MBED_CONFIG_PATH}/mbed_config.cmake)

mbed_add_cmake_directory_if_labels("TARGET")
