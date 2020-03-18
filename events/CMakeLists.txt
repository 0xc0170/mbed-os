# SPDX-License-Identifier: Apache-2.0

add_library(events STATIC)

target_link_libraries(events PUBLIC cmsis platform gen_config)

target_sources(events PRIVATE equeue.h)
target_sources(events PRIVATE Event.h)
target_sources(events PRIVATE EventQueue.h)
target_sources(events PRIVATE mbed_events.h)
target_sources(events PRIVATE mbed_shared_queues.h)
target_sources(events PRIVATE UserAllocatedEvent.h)

target_sources(events PRIVATE source/equeue.c)
target_sources(events PRIVATE source/equeue_mbed.cpp)
target_sources(events PRIVATE source/equeue_posix.c)
target_sources(events PRIVATE source/EventQueue.cpp)
target_sources(events PRIVATE source/mbed_shared_queues.cpp)

target_sources(events PRIVATE internal/equeue_platform.h)

target_include_directories(events PUBLIC .)
