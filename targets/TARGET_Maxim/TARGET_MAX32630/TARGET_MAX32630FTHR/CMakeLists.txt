# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-max32630fthr INTERFACE)
add_library(mbed-max32630fthr-obj OBJECT)

target_include_directories(mbed-max32630fthr 
    INTERFACE
        .
)

target_sources(mbed-max32630fthr-obj
    PRIVATE
        low_level_init.c
)

target_link_libraries(mbed-max32630fthr INTERFACE mbed-max32630)
target_link_libraries(mbed-max32630fthr-obj mbed-max32630-obj)
