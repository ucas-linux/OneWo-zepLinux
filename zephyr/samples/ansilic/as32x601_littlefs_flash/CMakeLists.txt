# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(as32x601_littlefs_flash)

# Add LittleFS library manually
target_sources(app PRIVATE
    src/main.c
    ${ZEPHYR_BASE}/../modules/fs/littlefs/lfs.c
    ${ZEPHYR_BASE}/../modules/fs/littlefs/lfs_util.c
)

target_include_directories(app PRIVATE
    ${ZEPHYR_BASE}/../modules/fs/littlefs
)

# Define required macros for LittleFS
target_compile_definitions(app PRIVATE
    CONFIG_FS_LOG_LEVEL=3
    LFS_NO_MALLOC
)
