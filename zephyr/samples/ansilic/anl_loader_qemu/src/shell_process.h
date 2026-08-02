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

typedef int (*cmd_exec_t)(int argc, char **argv);

struct shell_cmd {
	const char *name;
	const char *brief;
	cmd_exec_t exec;
};

struct cmd_data {
	int argc;
	char **argv;
	const struct shell_cmd *cmd;
	bool on_fg;
	struct k_sem copied_sem;
	int result;
};

pid_t new_task(const char *name, void *(*run)(void *), void *arg);
pid_t waitpid(pid_t pid, int *status, int options);
int shell_exec_command(const struct shell_cmd *cmd, int argc, char **argv, bool on_fg);
const struct shell_cmd *shell_cmd_lookup(const char *name);
int shell_cmd_register(const struct shell_cmd *cmd);
void shell_process_init(void);

#ifdef __cplusplus
}
#endif

#endif /* SHELL_PROCESS_H_ */
