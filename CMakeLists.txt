cmake_minimum_required(VERSION 3.28)

include(lib/pico_sdk_import.cmake)
include(lib/pico_extras_import_optional.cmake)

project(sparkle_fw16 C CXX ASM)

set(CMAKE_C_STANDARD 17)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.5.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.5.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

set(SKIP_PICO_STDIO_USB 1)
pico_sdk_init()

add_compile_options(
    -Wall
    -Wno-format
    -Wno-unused-function
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug"
OR  CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    add_compile_definitions(SPARKLE_ENABLE_LOGGING)
endif ()

if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-Wno-maybe-uninitialized)
endif()

add_compile_definitions(-DCFG_TUSB_CONFIG_FILE="${CMAKE_SOURCE_DIR}/include/drivers/usbcomm/tusb_config.h")

add_subdirectory(src)
target_include_directories(${PROJECT_NAME} PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)