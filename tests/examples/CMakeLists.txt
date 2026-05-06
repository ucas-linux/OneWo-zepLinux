# LMZ Example Applications

cmake_minimum_required(3.20)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

# Add LMZ library
add_subdirectory(../../code)

# Example applications
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/hello_lmz/CMakeLists.txt)
    add_subdirectory(hello_lmz)
endif()
