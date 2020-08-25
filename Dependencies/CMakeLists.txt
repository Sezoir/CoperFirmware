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

# Add units
set(DISABLE_PREDEFINED_UNITS ON)
set(DISABLE_IOSTREAM ON)
set(BUILD_TESTS OFF)
add_subdirectory(units)

# Add GSL support
add_subdirectory(GSL)
find_package(Microsoft.GSL CONFIG)


#########################################################
# Link libraries
#########################################################

## Link all the libraries together under the interface LibsModule
target_link_libraries(${Dependencies} INTERFACE
        units Microsoft.GSL::GSL
        )