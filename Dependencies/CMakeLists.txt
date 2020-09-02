## Set minimum cmake (note mbed-cmake requires at least CMake 3.12)
cmake_minimum_required(VERSION 3.15)


set(Dependencies LibsModule)

#########################################################
# Create a library representing a group of libraries
#########################################################

add_library(${Dependencies} INTERFACE)

#########################################################
# Get dependencies
#########################################################

# Add config packages to path
list(APPEND CMAKE_PREFIX_PATH ${ROOT_SOURCE_DIR}"/CMake/")

## Add units
# Declare types of units to use
set(DISABLE_PREDEFINED_UNITS ON)
set(ENABLE_PREDEFINED_LENGTH_UNITS ON)
set(ENABLE_PREDEFINED_MASS_UNITS ON)
set(ENABLE_PREDEFINED_TIME_UNITS ON)
set(ENABLE_PREDEFINED_ANGLE_UNITS ON)
set(ENABLE_PREDEFINED_TEMPERATURE_UNITS ON)
set(ENABLE_PREDEFINED_VELOCITY_UNITS ON)
set(ENABLE_PREDEFINED_ACCELERATION_UNITS ON)
set(ENABLE_PREDEFINED_MAGNETIC_UNITS ON)
set(ENABLE_PREDEFINED_CONSTANTS_UNITS ON)
# Other config options
set(DISABLE_IOSTREAM ON)
set(BUILD_TESTS OFF)
# Add units lib
add_subdirectory(units)

## Add GSL support
add_subdirectory(GSL)

#########################################################
# Link libraries
#########################################################

# Link all the libraries together under the interface LibsModule
target_link_libraries(${Dependencies} INTERFACE
        units Microsoft.GSL::GSL
        )