cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(priority_mapping_demo)

target_include_directories(app PRIVATE ../../../../code/include)
target_sources(app PRIVATE main.c ../../../../code/src/lmz_sched.c)
