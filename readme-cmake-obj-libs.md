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

This is similar approach to what Jamie proposed with `mbed_create_distro`. Each Mbed OS target would need to do similar.

## Converting our CMake targets to OBJECTS

This is cleaner way to achieve the goal but it requires our targets to be separted into objects and flags targets. 
Objects would contain sources that are built just once. And flags targets would be available to an application.

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

This will require to workaround our circular deps in our tree as CMake does not allow it with OBJECT libraries (it does for STATIC). This is known limitation and it might be fixed on day. For now we will need to do:

```
# work around circular reference with kvstore (CMake doesn't allow object libraries to link circularly)
# Apply flags from kvstore and add an interface dependency on it so they will get linked together later.
target_link_libraries(mbed-device_key INTERFACE mbed-storage-kvstore)
target_compile_definitions(mbed-device_key PRIVATE $<TARGET_PROPERTY:mbed-storage-kvstore,COMPILE_DEFINITIONS>)
target_include_directories(mbed-device_key PRIVATE $<TARGET_PROPERTY:mbed-storage-kvstore,INCLUDE_DIRECTORIES>)
```

