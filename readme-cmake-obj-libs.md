# How to build Mbed Os and its components just once for multiple targets

## Introducing the main components as OBJECT CMake targets

This is basically keeping the sources private to Mbed OS target and the rest expose as it might be required by an app (symbols, includes and compiler/linker options).

```
add_library(mbed-os OBJECT)
target_sources(mbed-os PRIVATE dummy.cpp)

target_link_libraries(mbed-os
    PRIVATE
        mbed-rtos
        mbed-core
        ${MBED_TARGET_CONVERTED}
)
```

We need to link as private, not exposing sources to an app. However, an app requires symbols or includes from Mbed OS. To fix this, we would need to expose these:

```
target_compile_options(mbed-os
    PUBLIC
        $<TARGET_PROPERTY:mbed-core,INTERFACE_COMPILE_OPTIONS>
        $<TARGET_PROPERTY:mbed-rtos,INTERFACE_COMPILE_OPTIONS>
        $<TARGET_PROPERTY:${MBED_TARGET_CONVERTED},INTERFACE_COMPILE_OPTIONS>
)

target_link_options(mbed-os
    PUBLIC
        $<TARGET_PROPERTY:mbed-core,INTERFACE_LINK_OPTIONS>
        $<TARGET_PROPERTY:mbed-rtos,INTERFACE_LINK_OPTIONS>
        $<TARGET_PROPERTY:${MBED_TARGET_CONVERTED},INTERFACE_LINK_OPTIONS>
)

target_compile_definitions(mbed-os
    PUBLIC
        $<TARGET_PROPERTY:mbed-core,INTERFACE_COMPILE_DEFINITIONS>
        $<TARGET_PROPERTY:mbed-rtos,INTERFACE_COMPILE_DEFINITIONS>
        $<TARGET_PROPERTY:${MBED_TARGET_CONVERTED},INTERFACE_COMPILE_DEFINITIONS>
)

target_include_directories(mbed-os
  PUBLIC
        $<TARGET_PROPERTY:mbed-core,INTERFACE_INCLUDE_DIRECTORIES>
        $<TARGET_PROPERTY:mbed-rtos,INTERFACE_INCLUDE_DIRECTORIES>
        $<TARGET_PROPERTY:${MBED_TARGET_CONVERTED},INTERFACE_INCLUDE_DIRECTORIES>
)
```

This is similar approach to what Jamie proposed with `mbed_create_distro`. Each Mbed OS CMake target would need to do similar (mbed-ble, mbed-nanostack, etc).

## Converting our CMake targets to OBJECTS

This is cleaner way to achieve the goal but it requires our targets to be separted into objects and flags targets.

Each CMake target would create xxx-flags and xxx-obj libraries.

```
# Contains everything but not sources
add_library(xxx-flags INTERFACE)
# Contains sources
add_libarry(xxx-obj OBJECT)
```

This is taken from Jamies prototype to illustrate how mbed-os/mbed-baremetal are formed.

```
# Interface library storing all needed flags, definitions, and includes for building with mbed-os.
# Does NOT include RTOS flags, so by itself it represents mbed-baremetal.
# It allows all optional modules to get the mbed flags without having to choose baremetal or RTOS specifically.
add_library(mbed-base-flags INTERFACE)

# RTOS flags only.  Linking mbed-base-flags plus mbed-rtos-flags provides the
# full-fat Mbed OS experience with the RTOS enabled
add_library(mbed-rtos-flags INTERFACE)

### mbed-baremetal
# private object library to build code
add_library(mbed-baremetal-obj OBJECT EXCLUDE_FROM_ALL)
target_link_libraries(mbed-baremetal-obj mbed-base-flags)

# public library containing flags and objects
add_library(mbed-baremetal INTERFACE)
target_sources(mbed-baremetal INTERFACE $<TARGET_OBJECTS:mbed-baremetal-obj>)
target_link_libraries(mbed-baremetal INTERFACE mbed-base-flags)

### mbed-os (baremetal plus RTOS)
# private object library to build code
add_library(mbed-os-obj OBJECT EXCLUDE_FROM_ALL)
target_link_libraries(mbed-os-obj mbed-base-flags mbed-rtos-flags)

# public library containing flags and objects
add_library(mbed-os INTERFACE)
target_sources(mbed-os INTERFACE $<TARGET_OBJECTS:mbed-os-obj>)
target_link_libraries(mbed-os INTERFACE mbed-base-flags mbed-rtos-flags)
```

There are at least 2 issue we need to work around.

1. Circular deps not supported with object libraries in CMake. Known issue that might be fixed one day.
2. Our mbed-rtos library affects the core (files check for `MBED_CONF_RTOS_PRESENT`)

Both were addressed by Jamie in some way to overcome these. 

To fix circular deps:

```
# work around circular reference with kvstore (CMake doesn't allow object libraries to link circularly)
# Apply flags from kvstore and add an interface dependency on it so they will get linked together later.
target_link_libraries(mbed-device_key INTERFACE mbed-storage-kvstore)
target_compile_definitions(mbed-device_key PRIVATE $<TARGET_PROPERTY:mbed-storage-kvstore,COMPILE_DEFINITIONS>)
target_include_directories(mbed-device_key PRIVATE $<TARGET_PROPERTY:mbed-storage-kvstore,INCLUDE_DIRECTORIES>)
```
