# Copyright (c) 2020-2021 ARM Limited. All rights reserved.
# SPDX-License-Identifier: Apache-2.0

add_library(mbed-802.15.4-rf INTERFACE)
add_library(mbed-lorawan INTERFACE)
add_library(mbed-wifi INTERFACE)

if(${CMAKE_CROSSCOMPILING})
    # The directories below contain optional target libraries
    add_subdirectory(FEATURE_BLE EXCLUDE_FROM_ALL)
    add_subdirectory(cellular EXCLUDE_FROM_ALL)

    add_subdirectory(libraries EXCLUDE_FROM_ALL)
    add_subdirectory(lorawan EXCLUDE_FROM_ALL)
    add_subdirectory(lwipstack EXCLUDE_FROM_ALL)
    add_subdirectory(mbedtls EXCLUDE_FROM_ALL)
    add_subdirectory(nanostack EXCLUDE_FROM_ALL)
    add_subdirectory(netsocket EXCLUDE_FROM_ALL)
    add_subdirectory(nfc EXCLUDE_FROM_ALL)
    add_subdirectory(drivers EXCLUDE_FROM_ALL)
else()
    # Add these subdirectories for the Unit test
    add_subdirectory(cellular)
    add_subdirectory(lorawan)
    add_subdirectory(netsocket)
endif()
