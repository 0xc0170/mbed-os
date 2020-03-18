# SPDX-License-Identifier: Apache-2.0

target_sources(target PRIVATE mbed_rtx.h)
target_sources(target PRIVATE USBEndpoints_Kinetis.h)
target_sources(target PRIVATE USBPhyHw.h)
target_sources(target PRIVATE USBPhy_Kinetis.cpp)

mbed_add_cmake_directory_if_labels("")

target_include_directories(target PUBLIC .)
