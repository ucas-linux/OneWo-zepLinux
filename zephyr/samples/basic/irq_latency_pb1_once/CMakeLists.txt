cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(irq_latency_pb1_once)

target_sources(app PRIVATE src/main.c src/devfd.c)
