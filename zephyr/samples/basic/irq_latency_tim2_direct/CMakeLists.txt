cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(irq_latency_tim2_direct)

target_sources(app PRIVATE src/main.c)
