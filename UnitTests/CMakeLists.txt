## Set minimum cmake
cmake_minimum_required(VERSION 3.15)

OPTION(ENABLE_UNIT_TESTS "Enables all unit tests." ON)

if (ENABLE_UNIT_TESTS)
    add_subdirectory(Motors)
    add_subdirectory(Sensors)
    add_subdirectory(Hovering)
endif (ENABLE_UNIT_TESTS)
