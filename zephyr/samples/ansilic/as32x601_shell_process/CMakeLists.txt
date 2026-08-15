# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(as32x601_shell_process)

target_sources(app PRIVATE
    src/main.c
    src/shell_process.c
    src/commands.c
    src/debug.c
    src/bytecode_vm.c
    src/anl_loader.c
)

# Add Phase 1 test support
target_sources(app PRIVATE
    ${ZEPHYR_BASE}/tests/kernel/process/test_phase1_fd_refcount.c
    ${ZEPHYR_BASE}/tests/kernel/process/test_phase1_shell.c
    ${ZEPHYR_BASE}/kernel/test_idesc.c
)
