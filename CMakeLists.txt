# Copyright (c) 2024 ARM Limited. All rights reserved.
# Copyright (c) 2024 Thomas Makin.
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.19.0)

set(MBED_PATH ${CMAKE_CURRENT_SOURCE_DIR}/mbed-os CACHE INTERNAL "")
set(MBED_CONFIG_PATH ${CMAKE_CURRENT_BINARY_DIR} CACHE INTERNAL "")
set(APP_TARGET e29-lorawan)

include(${MBED_PATH}/tools/cmake/app.cmake)

project(${APP_TARGET})

add_subdirectory(${MBED_PATH})

add_executable(${APP_TARGET})

file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*)
file(GLOB SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*)

target_include_directories(${APP_TARGET}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_sources(${APP_TARGET}
    PRIVATE
        main.cpp
        trace_helper.cpp
        ${SRC}
)

target_link_libraries(${APP_TARGET}
    PRIVATE
        mbed-os
        mbed-lorawan
)

mbed_set_post_build(${APP_TARGET})

option(VERBOSE_BUILD "Have a verbose build process")
if(VERBOSE_BUILD)
    set(CMAKE_VERBOSE_MAKEFILE ON)
endif()