# How to build Mbed Os and its components just once for multiple targets

Using CMake object libraries is not straight forward due to multiple known issues with it. It does not work the same as static libraries. 

## Components as CMake interface libraries with help of object libraries

Each Mbed OS CMake target would be turned into OBJECT library. Each subtarget can stay as interface.

```
# User facing library
add_library(mbed-ble INTERFACE)
# Internal object library to build sources just once
add_library(mbed-ble-obj OBJECT)

target_sources(mbed-ble-obj
    file.c
)

# Copy sources from object library
set_property(TARGET mbed-ble PROPERTY INTERFACE_SOURCES $<TARGET_OBJECTS:mbed-ble-obj>)

foreach(options COMPILE_DEFINITIONS COMPILE_FEATURES COMPILE_OPTIONS INCLUDE_DIRECTORIES LINK_LIBRARIES)
    set_target_properties(mbed-ble-obj PROPERTIES ${options} $<TARGET_PROPERTY:mbed-ble,INTERFACE_${options}>)
endforeach()
```

This is similar approach to what Jamie proposed with `mbed_create_distro`.

## Pure object libraries

Each component would need to be split into two separates CMake targets: 
- one providing objects (source files)
- one providing flags (compiler/linker options and macros)

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
3. It's not clear what needs to be linked where - a user porting targets might end up with very complicated tree (if a board inherits from like other 5 targets, to link everything together - flags + objects create lot of interdependencies)

Both were addressed by Jamie in some way to overcome these. 
