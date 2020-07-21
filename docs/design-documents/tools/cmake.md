# CMake Mbed OS

Requirements:
- CMake 3.13 and higher
- mbed-tools (python 3.6 and higher)

Two steps approach:

- Mbed OS core CMake
- Application CMake

Definitions and configurations would be defined in CMake files, Mbed app configuration file (`/path/to/app/mbed_app.json`) and Mbed library configuration files (`/path/to/mbed-os/<LIBRARY_NAME>/mbed_lib.json`). The Mbed build system would parse the Mbed configuration files and generate a `CMakeLists.txt` configuration file.

The following rules must be respected for backward compatibility.

We use target labels to include subfolders. A custom CMake function goes through the target labels. If there is a folder in the current CMake source directory matching a target label, it is included. 

An example, to add `TARGET_STM` in the a folder `targets`:

```
mbed_add_cmake_directory_if_labels("TARGET")
```

The same could be applied to other labels like features or components.

To migrate to the new build system, we can provide auto scanning of the module and generate CMake based on what we find or use the way as described above. In both cases, we could stay backward compatible.

## Mbed OS Core (Mbed OS repository)

There are numerous CMake files in the Mbed OS repository tree:

* The boiler plate for Mbed OS defined in Mbed OS root (provides way for the build system to overwrite/add configuration)
* Toolchain settings (toolchain.cmake) - configures the toolchain from cmake/toolchains directory based on the selected toolchain (`MBED_TOOLCHAIN`)
* Profile configuration (profile.cmake) - configures the profile from cmake/profiles directory based on the selected profile (`MBED_PROFILE`)
* Core configuration (core.cmake) - configures the core from cmake/cores directory based on the selected target (`MBED_CPU_CORE`)
* Utilities (util.cmake) - functions or macros we use
* Application cmake (app.cmake) - the application cmake
* Each component has a CMakeList file (describing the component - what files are to include, target/feature/component selection based on target, etc)

The next sections will describe static CMake files within Mbed OS Core repository.

### 1. Boilerplate CMake

The main CMakeList file in the Mbed OS repository contains boilerplate required to build Mbed OS. It describes the Mbed OS tree and provides all the options we have in Mbed OS.

This is not intended to be included by an application.

### 2. Toolchain CMake

All the toolchain settings are defined in the scripts found in `cmake/toolchains/`.

### 3. Profile CMake

The build profiles such as release or debug are defined in the scripts found in `cmake/profiles/`.

### 4. MCU core CMake

The MCU core definitions are defined in the scripts found in `cmake/cores/`.

### 5. Utilities

Custom functions/macros used within Mbed OS.

### 6. Application CMake

The CMake script that must be included by all applications using:

```
include(${MBED_ROOT}/cmake/app.cmake)
```

### 7. Component CMake

This file statically defines the structure of an Mbed OS component. It also contains conditional statements that are based on the configuration. Regular CMake expressions and Mbed OS functions/macros are used in it to conditionally include/exclude directories.
The rule of thumb is to not expose header files that are internal. We would like to avoid having everything in the include paths as we do now.

## Application CMake

We should provide application CMake functionality with  our own configuration. There are couple of approaches we could take. Statically defined CMake but then this disconnectes config and CMake - as CMake contains configuration for a project (like includes, sources, etc). Our build tool would need to parse CMake to get all paths used in the project or Mbed OS to find out where to look for configuration file. Therefore the build system has a knowledge as it is currently. We use `requires` to include/exclude modules.

By default, baremetal would be selected - requires set to hal, platform, drivers and cmsis. If an app needs anything else, would use `requires` in the config to include - BLE/networking/etc.

A user create own CMake file to configure an application, also with `mbed_app.json` configuration file. The building of an app would look like:

1. Parse the arguments provided to build command
2. Parse the application configuration
3. Get the target configuration
4. Get the Mbed OS configuration (select what modules we need and get their config, paths, etc)
5. Create .mbedbuild/mbed_config.cmake
6. Build an application
