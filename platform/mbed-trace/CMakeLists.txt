# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_subdirectory(source)

target_include_directories(mbed-baremetal
    INTERFACE
        include
        include/mbed-trace
)

target_sources(mbed-baremetal-obj
    PRIVATE
        source/mbed_trace.c
)
