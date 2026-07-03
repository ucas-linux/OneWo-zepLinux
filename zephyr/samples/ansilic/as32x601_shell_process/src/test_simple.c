/*
 * Simple test command that doesn't create a new thread
 */

#include <zephyr/kernel.h>
#include <zephyr/kernel/process.h>
#include <stdio.h>
#include "shell_process.h"

/**
 * @brief Simple PS command - runs in same thread
 */
static int cmd_ps_simple(int argc, char **argv)
{
	printk("=== SIMPLE PS (no thread creation) ===\n");
	printk("PID    PPID   Command\n");
	printk("------------------------\n");

	/* Get current process */
	struct z_process *current = process_current();
	if (!current || current->pid == PID_INVALID) {
		printk("ERROR: Invalid current process\n");
		return -1;
	}

	/* Print current process */
	printk("%-6d %-6d %s\n", (int)current->pid, 0, "ps_simple");

	/* Print init process */
	printk("%-6d %-6d %s\n", (int)PID_INIT, 0, "init");

	printk("=== SIMPLE PS DONE - returning ===\n");
	return 0;
}

/* Register command */
SHELL_CMD_REGISTER(ps_simple, "List processes (no thread)", cmd_ps_simple);
