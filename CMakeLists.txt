# Copyright (c) 2024 ARM Limited. All rights reserved.
# Copyright (c) 2024 Thomas Makin.
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.19.0)

set(MBED_PATH ${CMAKE_CURRENT_SOURCE_DIR}/mbed-os CACHE INTERNAL "")
set(MBED_CONFIG_PATH ${CMAKE_CURRENT_BINARY_DIR} CACHE INTERNAL "")

set(APP_TARGET e29-lorawan)

set(TARGET_RECEIVER e29-lorawan-rec)
set(TARGET_SENDER e29-lorawan-snd)

include(${MBED_PATH}/tools/cmake/app.cmake)

project(${APP_TARGET})

add_subdirectory(${MBED_PATH})

file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*)
file(GLOB SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*)

add_executable(${TARGET_RECEIVER})

target_include_directories(${TARGET_RECEIVER}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_sources(${TARGET_RECEIVER}
    PRIVATE
        main_receiver.cpp
        ${SRC}
)

target_link_libraries(${TARGET_RECEIVER}
    PRIVATE
        mbed-os
        mbed-lorawan
)

mbed_set_post_build(${TARGET_RECEIVER})

add_executable(${TARGET_SENDER})

target_include_directories(${TARGET_SENDER}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_sources(${TARGET_SENDER}
    PRIVATE
        main_sender.cpp
        ${SRC}
)

target_link_libraries(${TARGET_SENDER}
    PRIVATE
        mbed-os
        mbed-lorawan
)

mbed_set_post_build(${TARGET_SENDER})

option(VERBOSE_BUILD "Have a verbose build process")
if(VERBOSE_BUILD)
    set(CMAKE_VERBOSE_MAKEFILE ON)
endif()
