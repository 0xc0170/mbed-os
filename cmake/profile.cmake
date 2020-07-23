# Copyright (c) 2020 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

include(${MBED_ROOT}/cmake/profiles/${MBED_PROFILE}.cmake)

set(CMAKE_BUILD_TYPE "${MBED_PROFILE}" CACHE
    STRING "Choose the type of build." FORCE
)

set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Develop" "Release"
)
