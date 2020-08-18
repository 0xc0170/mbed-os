# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(hal STATIC)

include(${MBED_CONFIG_PATH}/mbed_config.cmake)

mbed_set_cpu_core_options(hal ${MBED_TOOLCHAIN})
mbed_set_toolchain_options(hal)
mbed_set_language_standard(hal)
mbed_set_profile_options(hal ${MBED_TOOLCHAIN})

target_include_directories(hal
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        usb
        include
        include/hal
)

target_sources(hal
    PRIVATE
        LowPowerTickerWrapper.cpp
        mbed_compat.c
        mbed_critical_section_api.c
        mbed_flash_api.c
        mbed_gpio.c
        mbed_gpio_irq.c
        mbed_itm_api.c
        mbed_lp_ticker_api.c
        mbed_lp_ticker_wrapper.cpp
        mbed_pinmap_common.c
        mbed_pinmap_default.cpp
        mbed_ticker_api.c
        mbed_us_ticker_api.c
        static_pinmap.cpp

        mpu/mbed_mpu_v7m.c
        mpu/mbed_mpu_v8m.c

        #usb/mbed_usb_phy.cpp
)

target_link_libraries(hal 
    PUBLIC
        targets
)

