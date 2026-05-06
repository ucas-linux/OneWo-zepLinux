# LMZ Tests
# CMakeLists.txt for building LMZ tests

cmake_minimum_required(3.20)

# Find Zephyr package
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

# Add LMZ library
add_subdirectory(../code)

# Unit tests
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/unit/CMakeLists.txt)
    add_subdirectory(unit)
endif()

# Integration tests
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/integration/CMakeLists.txt)
    add_subdirectory(integration)
endif()

# Stress tests
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/stress/CMakeLists.txt)
    add_subdirectory(stress)
endif()

# Example applications
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/examples/CMakeLists.txt)
    add_subdirectory(examples)
endif()
