# LMZ Integration Tests

cmake_minimum_required(3.20)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

# Add LMZ library
add_subdirectory(../../code)

zephyr_library(integration_tests)

target_sources(integration_tests PRIVATE
    test_process.c
)

target_link_libraries(integration_tests PRIVATE lmz)
