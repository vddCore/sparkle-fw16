cmake_minimum_required(VERSION 3.28)

include(pico_sdk_import.cmake)
include(pico_extras_import_optional.cmake)

project(sparkle_fw16 C CXX ASM)

set(CMAKE_C_STANDARD 17)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.3.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.3.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

set(SKIP_PICO_STDIO_USB 1)
pico_sdk_init()

add_compile_options(-Wall
        -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
        -Wno-unused-function # we have some for the docs that aren't called
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug"
OR  CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    add_compile_definitions(SPARKLE_ENABLE_LOGGING)
endif ()

if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-Wno-maybe-uninitialized)
endif()

add_subdirectory(src)