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
* `CMakeLists.txt` entry points in each Mbed OS module subdirectory, describing the build specification for a module or component

A number of CMake scripts are contained in the `mbed-os/cmake` directory:
* `toolchain.cmake` - selects the toolchain script from the `cmake/toolchains` directory, based on the value of the `MBED_TOOLCHAIN` variable
* `profile.cmake` - selects the profile script from the `cmake/profiles` directory, based on the value of the `MBED_PROFILE` variable
* core.cmake` - selects the core script from the `cmake/cores` directory, based on the value of the `MBED_CPU_CORE` variable
* `util.cmake` - custom CMake helper functions and macros
* `app.cmake` - contains part of the build specification for an application

The next sections will describe static CMake files within Mbed OS Core repository.

### 1. Mbed OS `CMakeLists.txt` Entry Point

The `CMakeLists.txt` entry point in the root of the Mbed OS repository contains the top level build specification for Mbed OS. This file also includes the auto generated `mbed_config.cmake` script, which is created by `mbed-tools`.

This is not intended to be included by an application.

### 2. Toolchain CMake Scripts

All the toolchain settings are defined in the scripts found in `cmake/toolchains/`.

### 3. Profile CMake Scripts

The build profiles such as release or debug are defined in the scripts found in `cmake/profiles/`.

### 4. MCU Core CMake Scripts

The MCU core definitions are defined in the scripts found in `cmake/cores/`.

### 5. Utilities CMake Scripts

Custom functions/macros used within Mbed OS.

### 6. Application CMake

The CMake script that must be included by all applications using:

```
include(${MBED_ROOT}/cmake/app.cmake)
```

### 7. Component `CMakeLists.txt` Entry Point

This file statically defines the structure of an Mbed OS component. It also contains conditional statements that are based on the configuration. Regular CMake expressions and Mbed OS functions/macros are used in it to conditionally include/exclude directories.
The rule of thumb is to not expose header files that are internal. We would like to avoid having everything in the include paths as we do now.

## Building an Application

`mbed-tools` is the next generation of command line tooling for Mbed OS. `mbed-tools` replaces the `mbed-cli` and the Python modules in the `mbed-os/tools` directory.

`mbed-tools` has consolidated all of the required modules to build Mbed OS, along with the command line interface, into a single Python package which can be installed using standard python packaging tools.

TBD?:
### Configuration

The main purpose of `mbed-tools` is to parse the Mbed configuration system's JSON files (`mbed_lib.json`, `mbed_app.json` and `targets.json`). The tool outputs a single CMake configuration script, which is included by `app.cmake` and `mbed-os/CMakeLists.txt`.

To generate the CMake config script (named `mbed_config.cmake`) the user can run the `configure` command:

`mbedtools configure -t <toolchain> -m <target>`

This will output `mbed_config.cmake` in a directory named `.mbedbuild` at the root of the program tree.

`mbed_config.cmake` contains several variable definitions used to select the toolchain, core and profile CMake scripts to be used in the build system generation:
* `MBED_TOOLCHAIN`
* `MBED_TARGET`
* `MBED_CPU_CORE`
* `MBED_PROFILE`

The tools also generate an `MBED_TARGET_LABELS` variable, containing the labels, components and feature definitions from `targets.json`, used to select the required Mbed OS components to be built.

The macro definitions parsed from the Mbed OS configuration system are also included in `mbed_config.cmake`. The decision was made to remove `mbed_config.h`.
- mbedignore
- Application output (what is being generated)

A user create own CMake file to configure an application, also with `mbed_app.json` configuration file. The building of an app would look like:

1. Parse the arguments provided to build command
1. Parse the application configuration
1. Get the target configuration
1. Get the Mbed OS configuration (select what modules we need and get their config, paths, etc)
1. Create .mbedbuild/mbed_config.cmake
1. Build an application
