# SPDX-License-Identifier: Apache-2.0

add_library(target STATIC)
target_link_libraries(target PUBLIC cmsis platform hal gen_config)

mbed_add_cmake_directory_if_labels("")
