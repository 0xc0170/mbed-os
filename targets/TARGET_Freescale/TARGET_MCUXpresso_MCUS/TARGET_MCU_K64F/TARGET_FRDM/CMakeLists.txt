# SPDX-License-Identifier: Apache-2.0

# TODO: this needs fixing, the scope does not work
include(${MBED_CONFIG_PATH}/mbed_config.cmake)

target_sources(targets
    PRIVATE
        crc.c
        fsl_clock_config.c
        fsl_phy.c
        mbed_overrides.c
        PeripheralPins.c
)

target_include_directories(targets
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
