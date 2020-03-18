# SPDX-License-Identifier: Apache-2.0

target_sources(target PRIVATE crc.c)
target_sources(target PRIVATE crc.h)
target_sources(target PRIVATE device.h)
target_sources(target PRIVATE fsl_clock_config.c)
target_sources(target PRIVATE fsl_clock_config.h)
target_sources(target PRIVATE fsl_phy.c)
target_sources(target PRIVATE fsl_phy.h)
target_sources(target PRIVATE mbed_overrides.c)
target_sources(target PRIVATE PeripheralNames.h)
target_sources(target PRIVATE PeripheralPinMaps.h)
target_sources(target PRIVATE PeripheralPins.c)
target_sources(target PRIVATE PinNames.h)

target_include_directories(target PUBLIC .)
