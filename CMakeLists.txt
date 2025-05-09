cmake_minimum_required(VERSION 4.0)

include("cmake/gcc-arm-none-eabi.cmake")

project(${PROJECT_NAME} LANGUAGES C CXX ASM)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(PROJECT_DIR ${CMAKE_SOURCE_DIR})
set(APP_DIR ${PROJECT_DIR}/app)
set(CMAKE_DIR ${PROJECT_DIR}/cmake)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

message("Build type: ${CMAKE_BUILD_TYPE}")

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

set(CMAKE_C_FLAGS_DEBUG "-O0 -g")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")

add_subdirectory(${CMAKE_DIR}/stm32cubemx)
add_subdirectory(${APP_DIR})

target_compile_options(stm32cubemx INTERFACE 
    -w
)