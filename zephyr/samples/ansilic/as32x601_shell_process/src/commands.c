/*
 * Copyright (c) 2024 OneWo-rtLinux Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Sample shell commands
 */

#include <zephyr/kernel.h>
#include <zephyr/kernel/process.h>
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
	printk("DEBUG: Calling process_current()...\n");
	struct z_process *current = process_current();
	printk("DEBUG: process_current() returned %p\n", current);

	if (!current || current->pid == PID_INVALID) {
		printk("ERROR: Invalid current process\n");
		return -1;
	}

	printk("DEBUG: Current process PID = %d\n", current->pid);

	/* Print current process */
	printk("%-6d %-6d %s\n", (int)current->pid, 0, "ps");

	/* Print init process */
	printk("%-6d %-6d %s\n", (int)PID_INIT, 0, "init");

	printk("DEBUG: cmd_ps about to return\n");
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
 * @brief Help command - list available commands
 */
static int cmd_help(int argc, char **argv)
{
	printk("Available commands:\n");
	printk("  help     - Show this help message\n");
	printk("  hello    - Print hello message\n");
	printk("  echo     - Echo arguments\n");
	printk("  ps       - List processes\n");
	printk("  getpid   - Show current process ID\n");
	printk("  info     - Show detailed process info\n");
	printk("  sleep    - Sleep for specified milliseconds\n");
	printk("  test     - Run process creation test\n");

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
SHELL_CMD_REGISTER(help, "Show available commands", cmd_help);
