/*
 * Copyright (c) 2025 UCAS
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/kernel/process.h>
#include <zephyr/shell/shell.h>
#include <stdio.h>
#include "shell_process.h"

/* Test process entry function */
static void *test_process_main(void *arg)
{
	int id = (int)(uintptr_t)arg;

	printk("Process %d: Started (PID=%d)\n", id, process_current()->pid);

	for (int i = 0; i < 5; i++) {
		printk("Process %d: Iteration %d\n", id, i);
		k_msleep(1000);
	}

	printk("Process %d: Exiting\n", id);
	return (void *)(intptr_t)0;
}

/* Command: spawn - creates a new process */
static int exec_spawn(int argc, char **argv)
{
	if (argc < 2) {
		printk("Usage: spawn <id>\n");
		return -1;
	}

	int id = 0;
	for (const char *p = argv[1]; *p; p++) {
		if (*p >= '0' && *p <= '9') {
			id = id * 10 + (*p - '0');
		}
	}

	/* Create a new process using new_task */
	char name[32];
	snprintf(name, sizeof(name), "proc_%d", id);

	pid_t pid = new_task(name, test_process_main, (void *)(uintptr_t)id);

	if (pid > 0) {
		printk("Created process %d with PID %d\n", id, pid);
		return 0;
	} else {
		printk("Failed to create process: %d\n", pid);
		return -1;
	}
}

/* Command: ps - list processes */
static int exec_ps(int argc, char **argv)
{
	printk("PID  PPID NAME\n");
	printk("---  ---- ----\n");

	/* List all processes */
	for (int i = 0; i < CONFIG_MAX_PROCESS_COUNT; i++) {
		struct z_process *proc = process_get(i);
		if (proc && proc->pid > 0) {
			printk("%-4d %-4d %s\n",
			       proc->pid,
			       proc->parent ? proc->parent->pid : 0,
			       proc->threads.next ? "active" : "zombie");
		}
	}

	return 0;
}

/* Command: test */
static int exec_test(int argc, char **argv)
{
	printk("QEMU Process Test - Working!\n");
	printk("Available commands: spawn, ps, test\n");
	printk("\n");
	printk("Current process PID: %d\n", process_current()->pid);
	return 0;
}

/* Shell handlers - bridge to process-based commands */
static int shell_spawn_handler(const struct shell *sh, size_t argc, char **argv)
{
	const struct shell_cmd *cmd = shell_cmd_lookup("spawn");
	if (!cmd) {
		shell_error(sh, "Command not found");
		return -1;
	}

	int ret = shell_exec_command(cmd, argc, argv, true);
	if (ret != 0) {
		shell_error(sh, "Command failed: %d", ret);
	}
	return ret;
}

static int shell_ps_handler(const struct shell *sh, size_t argc, char **argv)
{
	const struct shell_cmd *cmd = shell_cmd_lookup("ps");
	if (!cmd) {
		shell_error(sh, "Command not found");
		return -1;
	}

	int ret = shell_exec_command(cmd, argc, argv, true);
	return ret;
}

static int shell_test_handler(const struct shell *sh, size_t argc, char **argv)
{
	const struct shell_cmd *cmd = shell_cmd_lookup("test");
	if (!cmd) {
		shell_error(sh, "Command not found");
		return -1;
	}

	int ret = shell_exec_command(cmd, argc, argv, true);
	return ret;
}

/* Register Zephyr shell commands */
SHELL_CMD_ARG_REGISTER(spawn, NULL, "Spawn a test process", shell_spawn_handler, 2, 0);
SHELL_CMD_ARG_REGISTER(ps, NULL, "List processes", shell_ps_handler, 1, 0);
SHELL_CMD_ARG_REGISTER(test, NULL, "Test command", shell_test_handler, 1, 0);

/* Bytecode VM shell handlers */
static int shell_ls_handler(const struct shell *sh, size_t argc, char **argv)
{
	const struct shell_cmd *cmd = shell_cmd_lookup("ls");
	if (!cmd) {
		shell_error(sh, "Command not found");
		return -1;
	}
	return shell_exec_command(cmd, argc, argv, true);
}

static int shell_upload_handler(const struct shell *sh, size_t argc, char **argv)
{
	const struct shell_cmd *cmd = shell_cmd_lookup("upload");
	if (!cmd) {
		shell_error(sh, "Command not found");
		return -1;
	}
	return shell_exec_command(cmd, argc, argv, true);
}

static int shell_run_handler(const struct shell *sh, size_t argc, char **argv)
{
	const struct shell_cmd *cmd = shell_cmd_lookup("run");
	if (!cmd) {
		shell_error(sh, "Command not found");
		return -1;
	}
	return shell_exec_command(cmd, argc, argv, true);
}

static int shell_rm_handler(const struct shell *sh, size_t argc, char **argv)
{
	const struct shell_cmd *cmd = shell_cmd_lookup("rm");
	if (!cmd) {
		shell_error(sh, "Command not found");
		return -1;
	}
	return shell_exec_command(cmd, argc, argv, true);
}

/* Register bytecode VM shell commands */
SHELL_CMD_ARG_REGISTER(ls, NULL, "List available programs", shell_ls_handler, 1, 0);
SHELL_CMD_ARG_REGISTER(upload, NULL, "Upload bytecode program", shell_upload_handler, 3, 0);
SHELL_CMD_ARG_REGISTER(run, NULL, "Execute bytecode program", shell_run_handler, 2, 0);
SHELL_CMD_ARG_REGISTER(rm, NULL, "Delete bytecode program", shell_rm_handler, 2, 0);

/* Register custom commands */
static struct shell_cmd cmd_spawn = {
	.name = "spawn",
	.brief = "Spawn a test process",
	.exec = exec_spawn,
};

static struct shell_cmd cmd_ps = {
	.name = "ps",
	.brief = "List processes",
	.exec = exec_ps,
};

static struct shell_cmd cmd_test = {
	.name = "test",
	.brief = "Test command",
	.exec = exec_test,
};

int main(void)
{
	printk("\n=== QEMU Process Test ===\n");
	printk("Process model enabled\n");
	printk("Shell ready. Try: test, spawn 1, ps\n\n");

	/* Register commands */
	shell_cmd_register(&cmd_spawn);
	shell_cmd_register(&cmd_ps);
	shell_cmd_register(&cmd_test);

	return 0;
}
