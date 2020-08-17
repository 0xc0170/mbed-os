# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_sources(cmsis
    PRIVATE
        mbed_tz_context.c
)

target_include_directories(cmsis
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
