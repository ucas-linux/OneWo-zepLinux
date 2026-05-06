# LMZ Unit Tests

cmake_minimum_required(3.20)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

# Add LMZ library
add_subdirectory(../../code)

# Build unit tests as Zephyr test application
zephyr_library(unit_tests)

target_sources(unit_tests PRIVATE
    test_thread.c
    test_memory.c
    test_sched.c
)

# Link LMZ library
target_link_libraries(unit_tests PRIVATE lmz)
