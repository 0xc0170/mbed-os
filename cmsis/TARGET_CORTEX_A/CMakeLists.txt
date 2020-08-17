# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

target_sources(cmsis
    PRIVATE
        irq_ctrl_gic.c
)

if(${MBED_TOOLCHAIN} STREQUAL "IAR")
    target_sources(cmsis
        PRIVATE
            TOOLCHAIN_IAR/cmain.S
    )
endif()

target_include_directories(cmsis
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
