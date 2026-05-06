# LMZ Stress Tests

cmake_minimum_required(3.20)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

# Add LMZ library
add_subdirectory(../../code)

zephyr_library(stress_tests)

target_sources(stress_tests PRIVATE
    test_stress.c
)

target_link_libraries(stress_tests PRIVATE lmz)
