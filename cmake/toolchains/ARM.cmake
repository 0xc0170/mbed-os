# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

set(CMAKE_ASM_COMPILER "armclang")
set(CMAKE_C_COMPILER "armclang")
set(CMAKE_CXX_COMPILER "armclang")
set(CMAKE_AR "armar")
set(ARM_ELF2BIN "fromelf")
set_property(GLOBAL PROPERTY ELF2BIN ${ARM_ELF2BIN})

option(MBEDIDE "Use Arm compiler from Mbed Studio" OFF)
if(MBEDIDE)
    set_property(GLOBAL PROPERTY MBED_STUDIO_ARM_COMPILER "--ide=mbed")
endif()

# Get compile definitions - we need them for armasm
# ARMClang does not include defines to assembler as it does not preprocess files by default
set(_compile_definitions 
    "$<TARGET_PROPERTY:mbed-os,COMPILE_DEFINITIONS>"
)

set(_compile_definitions 
    "$<$<BOOL:${_compile_definitions}>:-D$<JOIN:${_compile_definitions}\,\", \"-D,>>"
)

function(generate_compile_definitions _filename)
  file(GENERATE OUTPUT "${_filename}" CONTENT "${_compile_definitions}\n")
endfunction()

generate_compile_definitions("compile_time_defs.txt")
set(_asm_macros "@compile_time_defs.txt")


# Sets toolchain options
function(mbed_set_toolchain_options target)
    get_property(mbed_studio_arm_compiler GLOBAL PROPERTY MBED_STUDIO_ARM_COMPILER)
    list(APPEND common_options
        "${mbed_studio_arm_compiler}"
        "-c"
        "--target=arm-arm-none-eabi"
        "-mthumb"
        "-Wno-armcc-pragma-push-pop"
        "-Wno-armcc-pragma-anon-unions"
        "-Wno-reserved-user-defined-literal"
        "-Wno-deprecated-register"
        "-fdata-sections"
        "-fno-exceptions"
        "-fshort-enums"
        "-fshort-wchar"
    )

    target_compile_options(${target}
        PUBLIC
            $<$<COMPILE_LANGUAGE:C>:${common_options}>
    )

    target_compile_options(${target}
        PUBLIC
            $<$<COMPILE_LANGUAGE:CXX>:${common_options}>
    )

    set(asm_preproc_options
        "--target=arm-arm-none-eabi"
    )
    target_compile_options(${target}
        PUBLIC
            $<$<COMPILE_LANGUAGE:ASM>:-masm=armasm ${_asm_macros}>
            $<$<COMPILE_LANGUAGE:ASM>:${MBED_STUDIO_ARM_COMPILER}>
            $<$<COMPILE_LANGUAGE:ASM>:${asm_preproc_options}>
    )

    target_compile_definitions(${target}
        PUBLIC
            TOOLCHAIN_ARM
    )

    target_link_options(${target}
        PUBLIC
            ${MBED_STUDIO_ARM_COMPILER}
    )
endfunction()
