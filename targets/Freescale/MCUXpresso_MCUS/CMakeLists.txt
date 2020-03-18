# SPDX-License-Identifier: Apache-2.0

#include(${MBED_OS_BASE}/util.cmake)

target_sources(target PRIVATE fsl_common.c)

target_sources(target PRIVATE api/analogin_api.c)
target_sources(target PRIVATE api/analogout_api.c)
target_sources(target PRIVATE api/dma_api.c)
target_sources(target PRIVATE api/dma_api_hal.h)
target_sources(target PRIVATE api/flash_api.c)
target_sources(target PRIVATE api/gpio_api.c)
target_sources(target PRIVATE api/gpio_irq_api.c)
target_sources(target PRIVATE api/gpio_object.h)
target_sources(target PRIVATE api/i2c_api.c)
target_sources(target PRIVATE api/lp_ticker.c)
target_sources(target PRIVATE api/objects.h)
target_sources(target PRIVATE api/PeripheralPins.h)
target_sources(target PRIVATE api/pinmap.c)
target_sources(target PRIVATE api/PortNames.h)
target_sources(target PRIVATE api/port_api.c)
target_sources(target PRIVATE api/qspi_api.c)
target_sources(target PRIVATE api/rtc_api.c)
target_sources(target PRIVATE api/sleep.c)

mbed_add_cmake_directory_if_labels("")

target_include_directories(target PUBLIC api)
