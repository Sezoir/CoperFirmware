## Set minimum cmake (note mbed-cmake requires at least CMake 3.12)
cmake_minimum_required(VERSION 3.15)

# Declare project
project(Hovering)

# Get all source and header files
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS
        ${CMAKE_SOURCE_DIR}/Src/Engine/*.cpp
        ${CMAKE_SOURCE_DIR}/Src/Control/*.cpp
        ${CMAKE_SOURCE_DIR}/Src/Drivers/*.cpp
        ${CMAKE_SOURCE_DIR}/Src/Sensors/*.cpp)

## Add executable
add_mbed_executable(${PROJECT_NAME}
        main.cpp
        ${SOURCE_FILES}
        ${CMAKE_SOURCE_DIR}/Src/Config.hpp
        ${CMAKE_SOURCE_DIR}/Src/Drivers/I2CInterface.hpp
        ${CMAKE_SOURCE_DIR}/Src/Drivers/I2CInterface.cpp
        )

# Include directories
target_include_directories(${PROJECT_NAME} PUBLIC
        ${PROJECT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/Src
        )

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

# Remove mbed register warning from log
add_definitions(-Wno-register -Wno-unknown-pragmas)

## Link all library module
target_link_libraries(${PROJECT_NAME} LibsModule)

