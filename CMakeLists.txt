## Set minimum cmake (note mbed-cmake requires at least CMake 3.12)
cmake_minimum_required(VERSION 3.15)

#########################################################
# Setup Mbed
#########################################################

## Include mbed project (Note we do this here instead of in dependencies as it is responsible for finding the compilers
## and building the project)
# Set upload method (using Clion openocd configuration so set to NONE)
set(UPLOAD_METHOD NONE)
set(UPLOAD_METHOD_DEFAULT NONE)
include(Dependencies/Mbed/mbed-cmake.cmake)


#########################################################
# Setup project
#########################################################

## ROOT project
project(ROOT)

# Add dependencies
add_subdirectory(Dependencies)

# Add project
add_subdirectory(Src)

# Add unit tests
add_subdirectory(UnitTests)

