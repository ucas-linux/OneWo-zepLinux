# LMZ Hello World Example

cmake_minimum_required(3.20)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

# Add LMZ library
add_subdirectory(../../../code)

zephyr_library(hello_lmz)

target_sources(hello_lmz PRIVATE
    main.c
)

target_link_libraries(hello_lmz PRIVATE lmz)
