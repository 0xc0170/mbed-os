# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

# Supported GCC ARM and ARM currently

if(MBED_OS_TOOLCHAIN STREQUAL "GCC_ARM")

# Mbed OS profiles inclusion

if(MBED_OS_TOOLCHAIN_PROFILE STREQUAL "DEVELOP")
include(${MBED_OS_ROOT}/cmake/toolchain_profiles/gcc_arm_develop.cmake)
elseif(MBED_OS_TOOLCHAIN_PROFILE STREQUAL "RELEASE")
include(${MBED_OS_ROOT}/cmake/toolchain_profiles/gcc_arm_release.cmake)
endif()

# TODO: @mbed-os-tools - this part should be generated and we get it included here
# include(${MBED_OS_APP_ROOT}/generated/generated_toolchain.cmake) for instance

# The profile extension by a target or an application

# TODO: @mbed-os-tools these extensions are from a target
set(CMAKE_C_FLAGS  ${CMAKE_C_FLAGS}
    "-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -DMBED_ROM_START=0x0 -DMBED_ROM_SIZE=0x100000 -DMBED_RAM1_START=0x1fff0000 -DMBED_RAM1_SIZE=0x10000 -DMBED_RAM_START=0x20000000 -DMBED_RAM_SIZE=0x30000"
)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS}
    "-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -DMBED_ROM_START=0x0 -DMBED_ROM_SIZE=0x100000 -DMBED_RAM1_START=0x1fff0000 -DMBED_RAM1_SIZE=0x10000 -DMBED_RAM_START=0x20000000 -DMBED_RAM_SIZE=0x30000"
)

set(CMAKE_ASM_FLAGS ${CMAKE_ASM_FLAGS}
    "-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -include mbed_config.h"
)

set(CMAKE_CXX_LINK_FLAGS ${CMAKE_CXX_LINK_FLAGS}
    "-Wl,-n -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -DMBED_ROM_START=0x0 -DMBED_ROM_SIZE=0x100000 -DMBED_RAM1_START=0x1fff0000 -DMBED_RAM1_SIZE=0x10000 -DMBED_RAM_START=0x20000000 -DMBED_RAM_SIZE=0x30000 -DMBED_BOOT_STACK_SIZE=1024 -DXIP_ENABLE=0"
)

set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} ${LD_SYS_LIBS} -T ${CMAKE_BINARY_DIR}/app.link_script.ld")

elseif(MBED_OS_TOOLCHAIN STREQUAL "ARM")

# Mbed OS profiles inclusion

if(MBED_OS_TOOLCHAIN_PROFILE STREQUAL "DEVELOP")
include(${MBED_OS_ROOT}/cmake/toolchain_profiles/arm_develop.cmake)
elseif(MBED_OS_TOOLCHAIN_PROFILE STREQUAL "RELEASE")
include(${MBED_OS_ROOT}/cmake/toolchain_profiles/arm_release.cmake)
endif()

# The profile extension by a target or an application

# TODO: @mbed tools to pass the processor type
set(CMAKE_SYSTEM_PROCESSOR    cortex-m4)

# TODO: @mbed-os-tools get flags from mbed-os/tools/profiles/,
#       mbed-os/tools/toolchains/arm.py, and target config in mbed-os/targets/targets.json
set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS}
    "-mfpu=none -mcpu=cortex-m4 -DMBED_ROM_START=0x0 -DMBED_ROM_SIZE=0x100000 -DMBED_RAM1_START=0x1fff0000 -DMBED_RAM1_SIZE=0x10000 -DMBED_RAM_START=0x20000000 -DMBED_RAM_SIZE=0x30000 -include mbed_config.h"
)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS}
    "-mthumb -DMBED_ROM_START=0x0 -DMBED_ROM_SIZE=0x100000 -DMBED_RAM1_START=0x1fff0000 -DMBED_RAM1_SIZE=0x10000 -DMBED_RAM_START=0x20000000 -DMBED_RAM_SIZE=0x30000  -include mbed_config.h"
)

set(CMAKE_ASM_FLAGS ${CMAKE_ASM_FLAGS}
    "--cpu=Cortex-M4 --cpreproc --cpreproc_opts=--target=arm-arm-none-eabi,-D,__FPU_PRESENT"
)

set(CMAKE_CXX_LINK_FLAGS ${CMAKE_CXX_LINK_FLAGS}
    "--predefine=-DMBED_ROM_START=0x0 --predefine=-DMBED_ROM_SIZE=0x100000 --predefine=-DMBED_RAM1_START=0x1fff0000 --predefine=-DMBED_RAM1_SIZE=0x10000 --predefine=-DMBED_RAM_START=0x20000000 --predefine=-DMBED_RAM_SIZE=0x30000 --predefine=-DMBED_BOOT_STACK_SIZE=1024 --predefine=-DXIP_ENABLE=0"
)

endif()
