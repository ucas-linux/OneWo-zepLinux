/*
 * Copyright (c) 2024 OneWo-rtLinux Team
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef SHELL_PROCESS_H_
#define SHELL_PROCESS_H_

#include <zephyr/kernel.h>
#include <zephyr/kernel/process.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Command execution function type
 *
 * Similar to Embox's cmd_exec_t
 */
typedef int (*cmd_exec_t)(int argc, char **argv);

/**
 * @brief Command descriptor
 *
 * Defines a shell command with its execution function
 */
struct shell_cmd {
	const char *name;        /* Command name */
	const char *brief;       /* Brief description */
	cmd_exec_t exec;         /* Execution function */
};

/**
 * @brief Command data structure
 *
 * Passed to new process when executing command.
 * Similar to Embox's struct cmd_data
 */
struct cmd_data {
	int argc;
	char **argv;
	const struct shell_cmd *cmd;
	bool on_fg;              /* Foreground execution flag */
	struct k_sem copied_sem; /* Semaphore for data copy synchronization */
	int result;              /* Command result code */
};

/**
 * @brief Create a new task/process
 *
 * Embox-compatible API for creating processes.
 * Creates a new process with its own thread.
 *
 * @param name Task name (can be empty string)
 * @param run Entry point function
 * @param arg Argument passed to entry point
 * @return Process ID (PID) on success, negative error code on failure
 */
pid_t new_task(const char *name, void *(*run)(void *), void *arg);

/**
 * @brief Wait for a process to complete
 *
 * Simplified waitpid implementation for foreground command execution.
 *
 * @param pid Process ID to wait for
 * @param status Pointer to store exit status (can be NULL)
 * @param options Wait options (currently unused)
 * @return PID on success, negative error code on failure
 */
pid_t waitpid(pid_t pid, int *status, int options);

/**
 * @brief Execute a shell command in a new process
 *
 * Similar to Embox's process_external() in tish.c
 *
 * @param cmd Command descriptor
 * @param argc Argument count
 * @param argv Argument array
 * @param on_fg True for foreground execution (wait), false for background
 * @return 0 on success, negative error code on failure
 */
int shell_exec_command(const struct shell_cmd *cmd, int argc, char **argv, bool on_fg);

/**
 * @brief Find a command by name
 *
 * @param name Command name to look up
 * @return Pointer to command descriptor or NULL if not found
 */
const struct shell_cmd *shell_cmd_lookup(const char *name);

/**
 * @brief Register a shell command
 *
 * @param cmd Command descriptor to register
 * @return 0 on success, negative error code on failure
 */
int shell_cmd_register(const struct shell_cmd *cmd);

/**
 * @brief Initialize shell process subsystem
 */
void shell_process_init(void);

/* Macro for command registration */
#define SHELL_CMD_REGISTER(_name, _brief, _exec) \
	static const struct shell_cmd _shell_cmd_##_name = { \
		.name = #_name, \
		.brief = _brief, \
		.exec = _exec, \
	}; \
	static int _shell_cmd_init_##_name(void) { \
		return shell_cmd_register(&_shell_cmd_##_name); \
	} \
	SYS_INIT(_shell_cmd_init_##_name, APPLICATION, \
		 CONFIG_KERNEL_INIT_PRIORITY_DEFAULT);

#ifdef __cplusplus
}
#endif

#endif /* SHELL_PROCESS_H_ */
