# CMake Mbed OS

Requirements:
- CMake 3.13 and higher
- mbed-tools (python 3.6 and higher)

Two steps approach:

- Mbed OS core CMake
- Building an application with mbed-tools

Definitions and configurations would be defined in CMake files, Mbed app configuration file (`/path/to/app/mbed_app.json`) and Mbed library configuration files (`/path/to/mbed-os/<LIBRARY_NAME>/mbed_lib.json`). The Mbed build system would parse the Mbed configuration files and generate a `CMakeLists.txt` configuration file.

The following rules must be respected for backward compatibility.

We use target labels to include subfolders. A custom CMake function goes through the target labels. If there is a folder in the current CMake source directory matching a target label, it is included. 

An example, to add `TARGET_STM` in the folder `targets` where we have folders like TARGET_NXP, TARGET_STM, etc:

```
mbed_add_cmake_directory_if_labels("TARGET")
```

If a user selects for example target `NUCLEO_F411RE`, the target defines the label `STM`. As result, the target folder STM is included.

The same could be applied to other labels like features or components.

To migrate to the new build system, we can provide auto scanning of the module and generate CMake based on what we find or use the way as described above. In both cases, we could stay backward compatible.

## Mbed OS Core (Mbed OS repository)

There are numerous CMake files in the Mbed OS repository tree:

* A `CMakeLists.txt` entry point in the Mbed OS root, describing the top level build specification for the Mbed OS source tree.
* Toolchain settings (toolchain.cmake) - configures the toolchain from cmake/toolchains directory based on the selected toolchain (`MBED_TOOLCHAIN`)
* Profile configuration (profile.cmake) - configures the profile from cmake/profiles directory based on the selected profile (`MBED_PROFILE`)
* Core configuration (core.cmake) - configures the core from cmake/cores directory based on the selected target (`MBED_CPU_CORE`)
* Utilities (util.cmake) - functions or macros we use
* Application cmake (app.cmake) - the application cmake
* Each component has a CMakeList file (describing the component - what files are to include, target/feature/component selection based on target, etc)

The next sections will describe static CMake files within Mbed OS Core repository.

### 1. Mbed OS `CMakeLists.txt` Entry Point

The `CMakeLists.txt` entry point in the root of the Mbed OS repository contains the top level build specification for Mbed OS. This file also includes the auto generated `mbed_config.cmake` script, which is created by `mbed-tools`.

This is not intended to be included by an application.

### 2. Toolchain CMake Scripts

All the toolchain settings are defined in the scripts found in `cmake/toolchains/`.

### 3. Profile CMake Scripts

The build profiles such as release or debug are defined in the scripts found in `cmake/profiles/`.

### 4. MCU core CMake

The MCU core definitions are defined in the scripts found in `cmake/cores/`.

### 5. Utilities CMake

Custom functions/macros used within Mbed OS.

### 6. Application CMake

The CMake script that must be included by all applications using:

```
include(${MBED_ROOT}/cmake/app.cmake)
```

### 7. Component CMake

This file statically defines the structure of an Mbed OS component. It also contains conditional statements that are based on the configuration. Regular CMake expressions and Mbed OS functions/macros are used in it to conditionally include/exclude directories.
The rule of thumb is to not expose header files that are internal. We would like to avoid having everything in the include paths as we do now.

## Building an Application

`mbed-tools` is the next generation of command line tooling for Mbed OS. `mbed-tools` replaces the `mbed-cli` and the Python modules in the `mbed-os/tools` directory.

`mbed-tools` has consolidated all of the required modules to build Mbed OS, along with the command line interface, into a single Python package which can be installed using standard python packaging tools.

TBD?:
- configuration
- mbedignore
- Application output (what is being generated)

A user create own CMake file to configure an application, also with `mbed_app.json` configuration file. The building of an app would look like:

1. Parse the arguments provided to build command
1. Parse the application configuration
1. Get the target configuration
1. Get the Mbed OS configuration (select what modules we need and get their config, paths, etc)
1. Create .mbedbuild/mbed_config.cmake
1. Build an application
