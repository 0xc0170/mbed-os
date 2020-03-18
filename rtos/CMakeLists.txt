# SPDX-License-Identifier: Apache-2.0

add_library(rtos STATIC)

target_link_libraries(rtos PUBLIC cmsis platform target drivers gen_config)

target_sources(rtos PRIVATE ConditionVariable.h)
target_sources(rtos PRIVATE EventFlags.h)
target_sources(rtos PRIVATE Kernel.h)
target_sources(rtos PRIVATE Mail.h)
target_sources(rtos PRIVATE mbed_rtos1_types.h)
target_sources(rtos PRIVATE mbed_rtos_storage.h)
target_sources(rtos PRIVATE mbed_rtos_types.h)
target_sources(rtos PRIVATE MemoryPool.h)
target_sources(rtos PRIVATE Mutex.h)
target_sources(rtos PRIVATE Queue.h)
target_sources(rtos PRIVATE rtos.h)
target_sources(rtos PRIVATE RtosTimer.h)
target_sources(rtos PRIVATE Semaphore.h)
target_sources(rtos PRIVATE ThisThread.h)
target_sources(rtos PRIVATE Thread.h)

target_sources(rtos PRIVATE source/ConditionVariable.cpp)
target_sources(rtos PRIVATE source/EventFlags.cpp)
target_sources(rtos PRIVATE source/Kernel.cpp)
target_sources(rtos PRIVATE source/Mutex.cpp)
target_sources(rtos PRIVATE source/RtosTimer.cpp)
target_sources(rtos PRIVATE source/rtos_handlers.h)
target_sources(rtos PRIVATE source/rtos_idle.h)
target_sources(rtos PRIVATE source/Semaphore.cpp)
target_sources(rtos PRIVATE source/ThisThread.cpp)
target_sources(rtos PRIVATE source/Thread.cpp)


add_subdirectory(source/TARGET_CORTEX)

target_include_directories(rtos PUBLIC .)

target_include_directories(rtos PRIVATE source)
