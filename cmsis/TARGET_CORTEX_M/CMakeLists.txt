# SPDX-License-Identifier: Apache-2.0

target_sources(cmsis PRIVATE cmsis_compiler.h)

target_sources(cmsis PRIVATE cmsis_version.h)
target_sources(cmsis PRIVATE core_armv81mml.h)
target_sources(cmsis PRIVATE core_armv8mbl.h)
target_sources(cmsis PRIVATE core_armv8mml.h)
target_sources(cmsis PRIVATE core_cm0plus.h)
target_sources(cmsis PRIVATE core_cm1.h)
target_sources(cmsis PRIVATE core_cm23.h)
target_sources(cmsis PRIVATE core_cm3.h)
target_sources(cmsis PRIVATE core_cm33.h)
target_sources(cmsis PRIVATE core_cm35p.h)
target_sources(cmsis PRIVATE core_cm4.h)
target_sources(cmsis PRIVATE core_cm7.h)
target_sources(cmsis PRIVATE core_sc000.h)
target_sources(cmsis PRIVATE core_sc300.h)
target_sources(cmsis PRIVATE mbed_tz_context.c)
target_sources(cmsis PRIVATE mpu_armv7.h)
target_sources(cmsis PRIVATE mpu_armv8.h)
target_sources(cmsis PRIVATE tz_context.h)

target_include_directories(cmsis PUBLIC .)
