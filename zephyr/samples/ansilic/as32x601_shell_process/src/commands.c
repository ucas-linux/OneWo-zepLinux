/*
 * Copyright (c) 2024 OneWo-rtLinux Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Sample shell commands
 */

#include <zephyr/kernel.h>
#include <zephyr/kernel/process.h>
#include <zephyr/sys/reboot.h>
#include <zephyr/version.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "shell_process.h"

/**
 * @brief Hello command - prints greeting with process info
 */
static int cmd_hello(int argc, char **argv)
{
	struct z_process *proc = process_current();

	if (!proc) {
		printk("Hello from unknown process (proc is NULL)!\n");
		return 0;
	}

	/* Validate process structure before accessing */
	if ((uintptr_t)proc < 0x20000000 || (uintptr_t)proc > 0x20080000) {
		printk("Hello from invalid process (proc=%p)!\n", proc);
		return 0;
	}

	pid_t my_pid = proc->pid;
	printk("Hello from process PID %d!\n", my_pid);

	if (argc > 1) {
		printk("Arguments: ");
		for (int i = 1; i < argc; i++) {
			printk("%s ", argv[i]);
		}
		printk("\n");
	}

	return 0;
}

/**
 * @brief Echo command - echoes arguments
 */
static int cmd_echo(int argc, char **argv)
{
	for (int i = 1; i < argc; i++) {
		printk("%s", argv[i]);
		if (i < argc - 1) {
			printk(" ");
		}
	}
	printk("\n");

	return 0;
}

/**
 * @brief PS command - list processes (simplified)
 */
static int cmd_ps(int argc, char **argv)
{
	printk("=== DEBUG: cmd_ps started ===\n");
	printk("PID    PPID   Command\n");
	printk("------------------------\n");

	/* Get current process */
	//printk("DEBUG: Calling process_current()...\n");
	struct z_process *current = process_current();
	//printk("DEBUG: process_current() returned %p\n", current);

	if (!current || current->pid == PID_INVALID) {
		printk("ERROR: Invalid current process\n");
		return -1;
	}

	//printk("DEBUG: Current process PID = %d\n", current->pid);

	/* Print current process */
	printk("%-6d %-6d %s\n", (int)current->pid, 0, "ps");

	/* Print init process */
	printk("%-6d %-6d %s\n", (int)PID_INIT, 0, "init");

	//printk("DEBUG: cmd_ps about to return\n");
	return 0;
}

/**
 * @brief Sleep command - sleeps for specified milliseconds
 */
static int cmd_sleep(int argc, char **argv)
{
	if (argc < 2) {
		printk("Usage: sleep <milliseconds>\n");
		return -EINVAL;
	}

	int ms = atoi(argv[1]);
	if (ms <= 0) {
		printk("Invalid sleep time: %s\n", argv[1]);
		return -EINVAL;
	}

	printk("Sleeping for %d ms in PID %d...\n",
	       ms, process_current() ? process_current()->pid : -1);
	k_msleep(ms);
	printk("Woke up!\n");

	return 0;
}

/**
 * @brief Test command - stress test for process creation
 */
static int cmd_test(int argc, char **argv)
{
	struct z_process *proc = process_current();

	printk("Test command running in PID %d\n", proc ? proc->pid : -1);
	printk("Creating 3 child processes...\n");

	for (int i = 0; i < 3; i++) {
		struct z_process *child = process_create(proc);
		if (child) {
			printk("  Created child process PID %d\n", child->pid);
			/* Clean up immediately for test */
			process_exit(child, 0);
		} else {
			printk("  Failed to create child %d\n", i);
		}
	}

	return 0;
}

/**
 * @brief Getpid command - print current process ID
 */
static int cmd_getpid(int argc, char **argv)
{
	struct z_process *proc = process_current();
	printk("Current PID: %d\n", proc ? proc->pid : -1);

	if (proc && proc->parent) {
		printk("Parent PID: %d\n", proc->parent->pid);
	}

	return 0;
}

/**
 * @brief Info command - print process information
 */
static int cmd_info(int argc, char **argv)
{
	struct z_process *proc = process_current();

	if (!proc) {
		printk("No current process!\n");
		return -ESRCH;
	}

	printk("Process Information:\n");
	printk("  PID: %d\n", proc->pid);
	printk("  Parent PID: %d\n", proc->parent ? proc->parent->pid : 0);
	printk("  Main thread: %p\n", proc->main_thread);
	printk("  Ref count: %ld\n", (long)atomic_get(&proc->ref_count));
	printk("  Exit code: %d\n", proc->exit_code);

	return 0;
}

/**
 * @brief Uptime command - show system uptime
 */
static int cmd_uptime(int argc, char **argv)
{
	int64_t uptime_ms = k_uptime_get();
	int64_t uptime_sec = uptime_ms / 1000;
	int hours = uptime_sec / 3600;
	int minutes = (uptime_sec % 3600) / 60;
	int seconds = uptime_sec % 60;

	printk("System uptime: %d hours, %d minutes, %d seconds\n",
	       hours, minutes, seconds);
	printk("Uptime (ms): %lld\n", uptime_ms);

	return 0;
}

/**
 * @brief Memory command - show memory usage information
 */
static int cmd_mem(int argc, char **argv)
{
	struct z_process *proc = process_current();

	printk("Memory Information:\n");

	if (proc) {
		printk("  Current Process PID: %d\n", proc->pid);
		if (proc->main_thread) {
			printk("  Main thread: %p\n", proc->main_thread);
		}
	}

#ifdef CONFIG_HEAP_MEM_POOL_SIZE
	printk("  Heap configured: %d bytes\n", CONFIG_HEAP_MEM_POOL_SIZE);
#endif

	return 0;
}

/**
 * @brief Clear command - clear screen (send ANSI escape codes)
 */
static int cmd_clear(int argc, char **argv)
{
	printk("\033[2J\033[H");
	return 0;
}

/**
 * @brief Version command - show system version
 */
static int cmd_version(int argc, char **argv)
{
	printk("OneWo-zepLinux\n");
	printk("Kernel version: %s\n", KERNEL_VERSION_STRING);
	printk("Board: as32x601_evb\n");
	printk("Architecture: RISC-V\n");

	return 0;
}

/**
 * @brief Reboot command - reboot the system
 */
static int cmd_reboot(int argc, char **argv)
{
	printk("Rebooting system...\n");
	k_msleep(100); /* Give time for message to be printed */

#ifdef CONFIG_REBOOT
	sys_reboot(SYS_REBOOT_COLD);
#else
	printk("Reboot not supported on this platform\n");
#endif
	return 0;
}

/**
 * @brief Date command - show current tick count
 */
static int cmd_date(int argc, char **argv)
{
	int64_t cycles = k_cycle_get_64();
	int64_t uptime_ms = k_uptime_get();

	printk("System ticks: %lld\n", k_uptime_ticks());
	printk("System cycles: %lld\n", cycles);
	printk("Uptime (ms): %lld\n", uptime_ms);

	return 0;
}

/**
 * @brief Free command - show free memory (simplified)
 */
static int cmd_free(int argc, char **argv)
{
	printk("Memory Usage (simplified):\n");

#ifdef CONFIG_HEAP_MEM_POOL_SIZE
	printk("  Total heap: %d bytes\n", CONFIG_HEAP_MEM_POOL_SIZE);
#else
	printk("  Heap not configured\n");
#endif

	/* Show stack usage if available */
	struct z_process *proc = process_current();
	if (proc && proc->main_thread) {
		size_t unused;
		int ret = k_thread_stack_space_get(proc->main_thread, &unused);
		if (ret == 0) {
			printk("  Thread stack unused: %zu bytes\n", unused);
		}
	}

	return 0;
}

/**
 * @brief Kill command - terminate a process by PID
 */
static int cmd_kill(int argc, char **argv)
{
	if (argc < 2) {
		printk("Usage: kill <pid>\n");
		return -EINVAL;
	}

	pid_t target_pid = atoi(argv[1]);

	if (target_pid == PID_INIT) {
		printk("Cannot kill init process!\n");
		return -EPERM;
	}

	struct z_process *proc = process_current();
	if (proc && proc->pid == target_pid) {
		printk("Terminating current process PID %d...\n", target_pid);
		process_exit(proc, 0);
		return 0;
	}

	printk("Process %d not found or not accessible\n", target_pid);
	return -ESRCH;
}

/**
 * @brief Benchmark command - simple CPU benchmark
 */
static int cmd_benchmark(int argc, char **argv)
{
	int iterations = 10000;

	if (argc > 1) {
		iterations = atoi(argv[1]);
		if (iterations <= 0 || iterations > 1000000) {
			printk("Invalid iterations. Using default (10000)\n");
			iterations = 10000;
		}
	}

	printk("Running benchmark with %d iterations...\n", iterations);

	int64_t start = k_uptime_get();
	volatile uint32_t sum = 0;

	for (int i = 0; i < iterations; i++) {
		sum += i * i;
	}

	int64_t end = k_uptime_get();
	int64_t elapsed = end - start;

	printk("Completed in %lld ms\n", elapsed);
	printk("Result: %u (to prevent optimization)\n", sum);

	return 0;
}

/**
 * @brief Stress command - create multiple processes rapidly
 */
static int cmd_stress(int argc, char **argv)
{
	int count = 5;

	if (argc > 1) {
		count = atoi(argv[1]);
		if (count <= 0 || count > 20) {
			printk("Count must be between 1 and 20\n");
			return -EINVAL;
		}
	}

	struct z_process *proc = process_current();
	if (!proc) {
		printk("No current process!\n");
		return -ESRCH;
	}

	printk("Creating %d processes rapidly...\n", count);
	int success = 0;
	int64_t start = k_uptime_get();

	for (int i = 0; i < count; i++) {
		struct z_process *child = process_create(proc);
		if (child) {
			success++;
			process_exit(child, 0);
		}
		k_msleep(10); /* Small delay */
	}

	int64_t elapsed = k_uptime_get() - start;
	printk("Created %d/%d processes in %lld ms\n", success, count, elapsed);

	return 0;
}

/**
 * @brief Help command - list available commands
 */
static int cmd_help(int argc, char **argv)
{
	printk("Available commands:\n");
	printk("  help       - Show this help message\n");
	printk("  hello      - Print hello message\n");
	printk("  echo       - Echo arguments\n");
	printk("  ps         - List processes\n");
	printk("  getpid     - Show current process ID\n");
	printk("  info       - Show detailed process info\n");
	printk("  sleep      - Sleep for specified milliseconds\n");
	printk("  test       - Run process creation test\n");
	printk("  uptime     - Show system uptime\n");
	printk("  mem        - Show memory information\n");
	printk("  free       - Show free memory\n");
	printk("  clear      - Clear screen\n");
	printk("  version    - Show system version\n");
	printk("  date       - Show current tick count\n");
	printk("  kill       - Terminate a process by PID\n");
	printk("  benchmark  - Run CPU benchmark\n");
	printk("  stress     - Stress test process creation\n");
	printk("  reboot     - Reboot the system\n");

	return 0;
}

/* Register commands using the macro */
SHELL_CMD_REGISTER(hello, "Print hello message", cmd_hello);
SHELL_CMD_REGISTER(echo, "Echo arguments", cmd_echo);
SHELL_CMD_REGISTER(ps, "List processes", cmd_ps);
SHELL_CMD_REGISTER(sleep, "Sleep for milliseconds", cmd_sleep);
SHELL_CMD_REGISTER(test, "Process creation test", cmd_test);
SHELL_CMD_REGISTER(getpid, "Show process ID", cmd_getpid);
SHELL_CMD_REGISTER(info, "Show process info", cmd_info);
SHELL_CMD_REGISTER(uptime, "Show system uptime", cmd_uptime);
SHELL_CMD_REGISTER(mem, "Show memory information", cmd_mem);
SHELL_CMD_REGISTER(free, "Show free memory", cmd_free);
SHELL_CMD_REGISTER(clear, "Clear screen", cmd_clear);
SHELL_CMD_REGISTER(version, "Show system version", cmd_version);
SHELL_CMD_REGISTER(date, "Show current tick count", cmd_date);
SHELL_CMD_REGISTER(kill, "Terminate a process", cmd_kill);
SHELL_CMD_REGISTER(benchmark, "Run CPU benchmark", cmd_benchmark);
SHELL_CMD_REGISTER(stress, "Stress test process creation", cmd_stress);
SHELL_CMD_REGISTER(reboot, "Reboot the system", cmd_reboot);
SHELL_CMD_REGISTER(help, "Show available commands", cmd_help);
