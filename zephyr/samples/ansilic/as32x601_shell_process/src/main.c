/*
 * Copyright (c) 2024 OneWo-rtLinux Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Main application - Shell with process-based command execution
 * Uses Zephyr shell subsystem for proper input handling
 */

#include <zephyr/kernel.h>
#include <zephyr/shell/shell.h>
#include <zephyr/kernel/process.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "shell_process.h"

/**
 * @brief Shell command handler - hello
 *
 * This creates a new process and executes the hello command
 */
static int shell_hello_handler(const struct shell *sh, size_t argc, char **argv)
{
	const struct shell_cmd *cmd = shell_cmd_lookup("hello");
	if (!cmd) {
		shell_error(sh, "Command not found");
		return -ENOENT;
	}

	/* Execute in new process (foreground) */
	int ret = shell_exec_command(cmd, argc, argv, true);
	if (ret != 0 && ret != -EINVAL) {
		shell_error(sh, "Command failed with code %d", ret);
	}

	return ret;
}

/**
 * @brief Shell command handler - echo
 */
static int shell_echo_handler(const struct shell *sh, size_t argc, char **argv)
{
	const struct shell_cmd *cmd = shell_cmd_lookup("echo");
	if (!cmd) {
		shell_error(sh, "Command not found");
		return -ENOENT;
	}

	int ret = shell_exec_command(cmd, argc, argv, true);
	if (ret != 0 && ret != -EINVAL) {
		shell_error(sh, "Command failed with code %d", ret);
	}

	return ret;
}

/**
 * @brief Shell command handler - ps
 */
static int shell_ps_handler(const struct shell *sh, size_t argc, char **argv)
{
	const struct shell_cmd *cmd = shell_cmd_lookup("ps");
	if (!cmd) {
		shell_error(sh, "Command not found");
		return -ENOENT;
	}

	int ret = shell_exec_command(cmd, argc, argv, true);
	if (ret != 0) {
		shell_error(sh, "Command failed with code %d", ret);
	}

	return ret;
}

/**
 * @brief Shell command handler - getpid
 */
static int shell_getpid_handler(const struct shell *sh, size_t argc, char **argv)
{
	const struct shell_cmd *cmd = shell_cmd_lookup("getpid");
	if (!cmd) {
		shell_error(sh, "Command not found");
		return -ENOENT;
	}

	int ret = shell_exec_command(cmd, argc, argv, true);
	if (ret != 0) {
		shell_error(sh, "Command failed with code %d", ret);
	}

	return ret;
}

/**
 * @brief Shell command handler - info
 */
static int shell_info_handler(const struct shell *sh, size_t argc, char **argv)
{
	const struct shell_cmd *cmd = shell_cmd_lookup("info");
	if (!cmd) {
		shell_error(sh, "Command not found");
		return -ENOENT;
	}

	int ret = shell_exec_command(cmd, argc, argv, true);
	if (ret != 0) {
		shell_error(sh, "Command failed with code %d", ret);
	}

	return ret;
}

/**
 * @brief Shell command handler - sleep
 */
static int shell_sleep_handler(const struct shell *sh, size_t argc, char **argv)
{
	const struct shell_cmd *cmd = shell_cmd_lookup("sleep");
	if (!cmd) {
		shell_error(sh, "Command not found");
		return -ENOENT;
	}

	int ret = shell_exec_command(cmd, argc, argv, true);
	if (ret != 0 && ret != -EINVAL) {
		shell_error(sh, "Command failed with code %d", ret);
	}

	return ret;
}

/**
 * @brief Shell command handler - test
 */
static int shell_test_handler(const struct shell *sh, size_t argc, char **argv)
{
	const struct shell_cmd *cmd = shell_cmd_lookup("test");
	if (!cmd) {
		shell_error(sh, "Command not found");
		return -ENOENT;
	}

	int ret = shell_exec_command(cmd, argc, argv, true);
	if (ret != 0) {
		shell_error(sh, "Command failed with code %d", ret);
	}

	return ret;
}

/**
 * @brief Shell command handler - help
 */
static int shell_help_handler(const struct shell *sh, size_t argc, char **argv)
{
	const struct shell_cmd *cmd = shell_cmd_lookup("help");
	if (!cmd) {
		shell_error(sh, "Command not found");
		return -ENOENT;
	}

	int ret = shell_exec_command(cmd, argc, argv, true);
	if (ret != 0) {
		shell_error(sh, "Command failed with code %d", ret);
	}

	return ret;
}

/**
 * @brief Shell command handler - ls (bytecode VM)
 */
static int shell_ls_handler(const struct shell *sh, size_t argc, char **argv)
{
	const struct shell_cmd *cmd = shell_cmd_lookup("ls");
	if (!cmd) {
		shell_error(sh, "Command not found");
		return -ENOENT;
	}

	int ret = shell_exec_command(cmd, argc, argv, true);
	if (ret != 0) {
		shell_error(sh, "Command failed with code %d", ret);
	}

	return ret;
}

/**
 * @brief Shell command handler - upload (bytecode VM)
 */
static int shell_upload_handler(const struct shell *sh, size_t argc, char **argv)
{
	const struct shell_cmd *cmd = shell_cmd_lookup("upload");
	if (!cmd) {
		shell_error(sh, "Command not found");
		return -ENOENT;
	}

	int ret = shell_exec_command(cmd, argc, argv, true);
	if (ret != 0) {
		shell_error(sh, "Command failed with code %d", ret);
	}

	return ret;
}

/**
 * @brief Shell command handler - upload_hex (bytecode VM)
 */
static int shell_upload_hex_handler(const struct shell *sh, size_t argc, char **argv)
{
	const struct shell_cmd *cmd = shell_cmd_lookup("upload_hex");
	if (!cmd) {
		shell_error(sh, "Command not found");
		return -ENOENT;
	}

	int ret = shell_exec_command(cmd, argc, argv, true);
	if (ret != 0) {
		shell_error(sh, "Command failed with code %d", ret);
	}

	return ret;
}

/**
 * @brief Shell command handler - run (bytecode VM)
 */
static int shell_run_handler(const struct shell *sh, size_t argc, char **argv)
{
	const struct shell_cmd *cmd = shell_cmd_lookup("run");
	if (!cmd) {
		shell_error(sh, "Command not found");
		return -ENOENT;
	}

	int ret = shell_exec_command(cmd, argc, argv, true);
	if (ret != 0) {
		shell_error(sh, "Command failed with code %d", ret);
	}

	return ret;
}

/**
 * @brief Shell command handler - rm (bytecode VM)
 */
static int shell_rm_handler(const struct shell *sh, size_t argc, char **argv)
{
	const struct shell_cmd *cmd = shell_cmd_lookup("rm");
	if (!cmd) {
		shell_error(sh, "Command not found");
		return -ENOENT;
	}

	int ret = shell_exec_command(cmd, argc, argv, true);
	if (ret != 0) {
		shell_error(sh, "Command failed with code %d", ret);
	}

	return ret;
}

/* Generic handler macro to reduce boilerplate */
#define DEFINE_SHELL_HANDLER(_handler_name, _cmd_name) \
static int _handler_name(const struct shell *sh, size_t argc, char **argv) \
{ \
	const struct shell_cmd *cmd = shell_cmd_lookup(_cmd_name); \
	if (!cmd) { \
		shell_error(sh, "Command not found"); \
		return -ENOENT; \
	} \
	int ret = shell_exec_command(cmd, argc, argv, true); \
	if (ret != 0 && ret != -EINVAL) { \
		shell_error(sh, "Command failed with code %d", ret); \
	} \
	return ret; \
}

DEFINE_SHELL_HANDLER(shell_uptime_handler, "uptime")
DEFINE_SHELL_HANDLER(shell_mem_handler, "mem")
DEFINE_SHELL_HANDLER(shell_free_handler, "free")
DEFINE_SHELL_HANDLER(shell_clear_handler, "clear")
DEFINE_SHELL_HANDLER(shell_version_handler, "version")
DEFINE_SHELL_HANDLER(shell_date_handler, "date")
DEFINE_SHELL_HANDLER(shell_kill_handler, "kill")
DEFINE_SHELL_HANDLER(shell_benchmark_handler, "benchmark")
DEFINE_SHELL_HANDLER(shell_stress_handler, "stress")
DEFINE_SHELL_HANDLER(shell_reboot_handler, "reboot")
DEFINE_SHELL_HANDLER(shell_fork_handler, "fork")

/* Register shell commands - these bridge to our process-based execution */
SHELL_CMD_ARG_REGISTER(hello, NULL, "Print hello message (runs in new process)",
                       shell_hello_handler, 1, 10);
SHELL_CMD_ARG_REGISTER(echo, NULL, "Echo arguments (runs in new process)",
                       shell_echo_handler, 1, 10);
SHELL_CMD_ARG_REGISTER(ps, NULL, "List processes (runs in new process)",
                       shell_ps_handler, 1, 0);
SHELL_CMD_ARG_REGISTER(getpid, NULL, "Show process ID (runs in new process)",
                       shell_getpid_handler, 1, 0);
SHELL_CMD_ARG_REGISTER(info, NULL, "Show process info (runs in new process)",
                       shell_info_handler, 1, 0);
SHELL_CMD_ARG_REGISTER(sleep, NULL, "Sleep for milliseconds (runs in new process)",
                       shell_sleep_handler, 2, 0);
SHELL_CMD_ARG_REGISTER(test, NULL, "Process creation test (runs in new process)",
                       shell_test_handler, 1, 0);
SHELL_CMD_ARG_REGISTER(prochelp, NULL, "Show process commands (runs in new process)",
                       shell_help_handler, 1, 0);
SHELL_CMD_ARG_REGISTER(uptime, NULL, "Show system uptime",
                       shell_uptime_handler, 1, 0);
SHELL_CMD_ARG_REGISTER(mem, NULL, "Show memory information",
                       shell_mem_handler, 1, 0);
SHELL_CMD_ARG_REGISTER(free, NULL, "Show free memory",
                       shell_free_handler, 1, 0);
SHELL_CMD_ARG_REGISTER(clear, NULL, "Clear screen",
                       shell_clear_handler, 1, 0);
SHELL_CMD_ARG_REGISTER(version, NULL, "Show system version",
                       shell_version_handler, 1, 0);
SHELL_CMD_ARG_REGISTER(date, NULL, "Show current tick count",
                       shell_date_handler, 1, 0);
SHELL_CMD_ARG_REGISTER(kill, NULL, "Terminate a process by PID",
                       shell_kill_handler, 2, 0);
SHELL_CMD_ARG_REGISTER(benchmark, NULL, "Run CPU benchmark",
                       shell_benchmark_handler, 1, 1);
SHELL_CMD_ARG_REGISTER(stress, NULL, "Stress test process creation",
                       shell_stress_handler, 1, 1);
SHELL_CMD_ARG_REGISTER(reboot, NULL, "Reboot the system",
                       shell_reboot_handler, 1, 0);

/* Bytecode VM shell commands */
SHELL_CMD_ARG_REGISTER(ls, NULL, "List programs and commands",
                       shell_ls_handler, 1, 0);
SHELL_CMD_ARG_REGISTER(upload, NULL, "Upload bytecode program",
                       shell_upload_handler, 3, 0);
SHELL_CMD_ARG_REGISTER(upload_hex, NULL, "Upload bytecode from hex string",
                       shell_upload_hex_handler, 3, 0);
SHELL_CMD_ARG_REGISTER(run, NULL, "Execute bytecode program",
                       shell_run_handler, 2, 0);
SHELL_CMD_ARG_REGISTER(rm, NULL, "Delete bytecode program",
                       shell_rm_handler, 2, 0);
SHELL_CMD_ARG_REGISTER(fork, NULL, "Fork child processes",
                       shell_fork_handler, 1, 2);

/**
 * @brief Main application entry point
 */
int main(void)
{
	printk("\n");
	printk("========================================\n");
	printk("  AS32X601 Shell with Process Support\n");
	printk("  Based on Embox process model\n");
	printk("========================================\n");
	printk("\n");

	/* Get init process info */
	struct z_process *init_proc = process_current();
	if (init_proc) {
		printk("Init process PID: %d\n", init_proc->pid);
	} else {
		printk("Warning: No init process found\n");
	}

	printk("\n");
	printk("Available commands:\n");
	printk("  echo      - Echo arguments\n");
	printk("  version   - Show system version\n");
	printk("  uptime    - Show system uptime\n");
	printk("  mem       - Show memory information\n");
	printk("  free      - Show free memory\n");
	printk("  benchmark - Run CPU benchmark\n");
	printk("  ps        - List processes\n");
	printk("  hello     - Print hello message\n");
	printk("  getpid    - Show process ID\n");
	printk("  info      - Show process info\n");
	printk("  sleep     - Sleep for milliseconds\n");
	printk("  test      - Process creation test\n");
	printk("  kill      - Terminate a process\n");
	printk("  stress    - Stress test process creation\n");
	printk("  fork      - Fork child processes\n");
	printk("  clear     - Clear screen\n");
	printk("  date      - Show current tick count\n");
	printk("  reboot    - Reboot the system\n");
	printk("\n");
	printk("Each command runs in a separate process.\n");
	printk("\n");

	/* Return to let Zephyr shell handle input */
	return 0;
}
