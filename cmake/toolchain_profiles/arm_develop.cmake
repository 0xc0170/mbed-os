# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# Mbed OS develop profile for ARMClang6 toolchain

set(CMAKE_ASM_COMPILER    "armasm")
set(CMAKE_C_COMPILER      "armclang")
set(CMAKE_CXX_COMPILER    "armclang")
set(CMAKE_AR              "armar")
set(ELF2BIN               "fromelf")

set(CMAKE_C_FLAGS
    "--target=arm-arm-none-eabi -g -O1 -Wno-armcc-pragma-push-pop -Wno-armcc-pragma-anon-unions -Wno-reserved-user-defined-literal -Wno-deprecated-register -DMULADDC_CANNOT_USE_R7 -fdata-sections -fno-exceptions -fshort-enums -fshort-wchar -DMBED_DEBUG -DMBED_TRAP_ERRORS_ENABLED=1 -D__ASSERT_MSG -std=gnu11 -include mbed_config.h"
)

set(CMAKE_CXX_FLAGS
    "--target=arm-arm-none-eabi -g -O1 -Wno-armcc-pragma-push-pop -Wno-armcc-pragma-anon-unions -Wno-reserved-user-defined-literal -Wno-deprecated-register -DMULADDC_CANNOT_USE_R7 -fdata-sections -fno-exceptions -fshort-enums -fshort-wchar -DMBED_DEBUG -DMBED_TRAP_ERRORS_ENABLED=1 -fno-rtti -fno-c++-static-destructors -std=gnu++14 -include mbed_config.h"
)

set(CMAKE_ASM_FLAGS
    "--cpreproc --cpreproc_opts=--target=arm-arm-none-eabi"
)

set(CMAKE_CXX_LINK_FLAGS
    "--verbose --remove --show_full_path --legacyalign --any_contingency --keep=os_cb_sections"
)

set(LD_SYS_LIBS 
    "-Wl,--start-group -lstdc++ -lsupc++ -lm -lc -lgcc -lnosys  -Wl,--end-group"
)
