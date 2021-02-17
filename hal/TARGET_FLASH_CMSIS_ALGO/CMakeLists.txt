# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(mbed-baremetal
    INTERFACE
        .
)

target_sources(mbed-baremetal-obj
    PRIVATE
        flash_common_algo.c
)
