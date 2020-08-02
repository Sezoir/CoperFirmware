## Set minimum cmake (note mbed-cmake requires at least CMake 3.12)
cmake_minimum_required(VERSION 3.15)

## Load project upload methods (OpenOCD, etc)
#include(CMake/UploadMethodConfig.cmake)

## Load compiler and flags
set(CMAKE_CXX_STANDARD 17)

## Include mbed project
# Set upload method (using Clion openocd configuration so set to NONE)
set(UPLOAD_METHOD NONE)
include(Dependencies/Mbed/mbed-cmake.cmake)

## Project
project(Copter)

## Add executable
add_mbed_executable(Copter main.cpp)