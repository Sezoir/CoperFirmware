## Set minimum cmake (note mbed-cmake requires at least CMake 3.12)
cmake_minimum_required(VERSION 3.15)

# Declare project
project(Copter)

# Get all source and header files
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS *.cpp *.c)
#file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS *.hpp *.h)

## Add executable
add_mbed_executable(${PROJECT_NAME}
        ${SOURCE_FILES}
        )

# Include directories
target_include_directories(${PROJECT_NAME} PUBLIC
        Src
#        ${HEADER_FILES}
        )

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17 )

# Remove mbed register warning from log
add_definitions(-Wno-register)

## Link all library module
target_link_libraries(${PROJECT_NAME} LibsModule)

