/*
 * Copyright (c) 2024 OneWo-rtLinux Team
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/kernel/process.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include "shell_process.h"

#define MAX_COMMANDS 16
#define TASK_STACK_SIZE 2048

static const struct shell_cmd *command_registry[MAX_COMMANDS];
static int command_count = 0;
static struct k_mutex cmd_registry_lock;

struct process_exit_msg {
	pid_t pid;
	int exit_code;
};

K_MSGQ_DEFINE(process_exit_queue, sizeof(struct process_exit_msg), 16, 4);

#define MAX_STACK_POOL 3
K_THREAD_STACK_ARRAY_DEFINE(stack_pool, MAX_STACK_POOL, TASK_STACK_SIZE);
static bool stack_pool_used[MAX_STACK_POOL];
static struct k_mutex stack_pool_lock;

static struct {
	pid_t pid;
	int stack_index;
	struct k_thread *thread;
} stack_tracker[MAX_STACK_POOL];
static struct k_mutex stack_tracker_lock;

struct task_trampoline_arg {
	void *(*run)(void *);
	void *run_arg;
};

static void task_trampoline(void *arg1, void *arg2, void *arg3)
{
	struct task_trampoline_arg *arg = (struct task_trampoline_arg *)arg1;

	k_msleep(10);

	if (!arg || !arg->run) return;

	void *result = arg->run(arg->run_arg);

	struct z_process *proc = process_current();
	if (proc) {
		struct process_exit_msg msg = {
			.pid = proc->pid,
			.exit_code = (int)(intptr_t)result,
		};
		k_msgq_put(&process_exit_queue, &msg, K_NO_WAIT);
	}

	k_free(arg);
	k_yield();
	k_msleep(100);
}

pid_t new_task(const char *name, void *(*run)(void *), void *arg)
{
	if (!run) return -EINVAL;

	struct z_process *parent = process_current();
	if (!parent) return -ESRCH;

	struct z_process *child = process_create(parent);
	if (!child) return -ENOMEM;

	k_thread_stack_t *stack = NULL;
	int stack_index = -1;

	k_mutex_lock(&stack_pool_lock, K_FOREVER);
	for (int i = 0; i < MAX_STACK_POOL; i++) {
		if (!stack_pool_used[i]) {
			stack_pool_used[i] = true;
			stack = &stack_pool[i][0];
			stack_index = i;
			break;
		}
	}
	k_mutex_unlock(&stack_pool_lock);

	if (!stack) {
		process_exit(child, -ENOMEM);
		return -ENOMEM;
	}

	struct k_thread *thread = k_malloc(sizeof(struct k_thread));
	if (!thread) {
		k_mutex_lock(&stack_pool_lock, K_FOREVER);
		stack_pool_used[stack_index] = false;
		k_mutex_unlock(&stack_pool_lock);
		process_exit(child, -ENOMEM);
		return -ENOMEM;
	}

	struct task_trampoline_arg *tramp_arg = k_malloc(sizeof(struct task_trampoline_arg));
	if (!tramp_arg) {
		k_free(thread);
		k_mutex_lock(&stack_pool_lock, K_FOREVER);
		stack_pool_used[stack_index] = false;
		k_mutex_unlock(&stack_pool_lock);
		process_exit(child, -ENOMEM);
		return -ENOMEM;
	}

	tramp_arg->run = run;
	tramp_arg->run_arg = arg;

	process_register_thread(child, thread);

	k_tid_t tid = k_thread_create(
		thread, stack,
		K_THREAD_STACK_SIZEOF(TASK_STACK_SIZE),
		task_trampoline, tramp_arg, NULL, NULL,
		K_PRIO_PREEMPT(7), 0, K_NO_WAIT);

	if (!tid) {
		process_unregister_thread(child, thread);
		k_free(tramp_arg);
		k_free(thread);
		k_mutex_lock(&stack_pool_lock, K_FOREVER);
		stack_pool_used[stack_index] = false;
		k_mutex_unlock(&stack_pool_lock);
		process_exit(child, -EAGAIN);
		return -EAGAIN;
	}

	k_mutex_lock(&stack_tracker_lock, K_FOREVER);
	for (int i = 0; i < MAX_STACK_POOL; i++) {
		if (stack_tracker[i].pid == 0) {
			stack_tracker[i].pid = child->pid;
			stack_tracker[i].stack_index = stack_index;
			stack_tracker[i].thread = thread;
			break;
		}
	}
	k_mutex_unlock(&stack_tracker_lock);

	if (name && name[0] != '\0') {
		k_thread_name_set(tid, name);
	}

	return child->pid;
}

pid_t waitpid(pid_t pid, int *status, int options)
{
	struct process_exit_msg msg;

	while (1) {
		int ret = k_msgq_get(&process_exit_queue, &msg, K_FOREVER);
		if (ret == 0) {
			if (msg.pid == pid || pid == -1) {
				if (status) *status = msg.exit_code;

				struct z_process *proc = process_get(msg.pid);
				if (proc && proc->main_thread) {
					int stack_index = -1;
					struct k_thread *thread = proc->main_thread;

					k_mutex_lock(&stack_tracker_lock, K_FOREVER);
					for (int i = 0; i < MAX_STACK_POOL; i++) {
						if (stack_tracker[i].pid == msg.pid) {
							stack_index = stack_tracker[i].stack_index;
							stack_tracker[i].pid = 0;
							stack_tracker[i].stack_index = -1;
							stack_tracker[i].thread = NULL;
							break;
						}
					}
					k_mutex_unlock(&stack_tracker_lock);

					k_thread_join(proc->main_thread, K_FOREVER);
					process_unregister_thread(proc, thread);
					k_free(thread);

					if (stack_index >= 0 && stack_index < MAX_STACK_POOL) {
						k_mutex_lock(&stack_pool_lock, K_FOREVER);
						stack_pool_used[stack_index] = false;
						k_mutex_unlock(&stack_pool_lock);
					}

					process_exit(proc, msg.exit_code);
				}
				return msg.pid;
			}
			k_msgq_put(&process_exit_queue, &msg, K_NO_WAIT);
			k_yield();
		}
	}

	return -ECHILD;
}

const struct shell_cmd *shell_cmd_lookup(const char *name)
{
	if (!name) return NULL;

	k_mutex_lock(&cmd_registry_lock, K_FOREVER);
	for (int i = 0; i < command_count; i++) {
		if (command_registry[i] && strcmp(command_registry[i]->name, name) == 0) {
			k_mutex_unlock(&cmd_registry_lock);
			return command_registry[i];
		}
	}
	k_mutex_unlock(&cmd_registry_lock);
	return NULL;
}

int shell_cmd_register(const struct shell_cmd *cmd)
{
	if (!cmd || !cmd->name || !cmd->exec) return -EINVAL;

	k_mutex_lock(&cmd_registry_lock, K_FOREVER);
	if (command_count >= MAX_COMMANDS) {
		k_mutex_unlock(&cmd_registry_lock);
		return -ENOMEM;
	}
	command_registry[command_count++] = cmd;
	k_mutex_unlock(&cmd_registry_lock);
	return 0;
}

void shell_process_init(void)
{
	k_mutex_init(&cmd_registry_lock);
	k_mutex_init(&stack_tracker_lock);
	k_mutex_init(&stack_pool_lock);
	command_count = 0;
	memset(command_registry, 0, sizeof(command_registry));
	memset(stack_tracker, 0, sizeof(stack_tracker));
	memset(stack_pool_used, 0, sizeof(stack_pool_used));
}

static int shell_process_init_wrapper(void)
{
	shell_process_init();
	return 0;
}

SYS_INIT(shell_process_init_wrapper, APPLICATION, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT);
