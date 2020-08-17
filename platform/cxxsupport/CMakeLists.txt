# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_include_directories(platform
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(platform
    PRIVATE
        mstd_mutex.cpp
)
