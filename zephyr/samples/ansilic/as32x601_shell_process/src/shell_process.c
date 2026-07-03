/*
 * Copyright (c) 2024 OneWo-rtLinux Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Shell Process Execution Implementation
 * Based on Embox's tish.c and new_task() mechanism
 */

#include <zephyr/kernel.h>
#include <zephyr/kernel/process.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include "shell_process.h"

#define MAX_COMMANDS 32
#define TASK_STACK_SIZE 2048

/* Command registry */
static const struct shell_cmd *command_registry[MAX_COMMANDS];
static int command_count = 0;
static struct k_mutex cmd_registry_lock;

/* Process exit message structure */
struct process_exit_msg {
	pid_t pid;
	int exit_code;
};

/* Process wait queue for waitpid() */
K_MSGQ_DEFINE(process_exit_queue, sizeof(struct process_exit_msg), 16, 4);

/* Stack tracking for cleanup - maps PID to stack pointer */
#define MAX_TRACKED_STACKS 16

/* Static stack pool - must use K_THREAD_STACK_DEFINE for proper alignment */
#define MAX_STACK_POOL 8
K_THREAD_STACK_ARRAY_DEFINE(stack_pool, MAX_STACK_POOL, TASK_STACK_SIZE);
static bool stack_pool_used[MAX_STACK_POOL];
static struct k_mutex stack_pool_lock;

static struct {
	pid_t pid;
	int stack_index;  /* Index into stack_pool instead of pointer */
	struct k_thread *thread;
} stack_tracker[MAX_TRACKED_STACKS];
static struct k_mutex stack_tracker_lock;

/* Thread entry point trampoline */
struct task_trampoline_arg {
	void *(*run)(void *);
	void *run_arg;
};

/**
 * @brief Task trampoline - wraps user function
 *
 * Similar to Embox's task_trampoline() in multi.c
 */
static void task_trampoline(void *arg1, void *arg2, void *arg3)
{
	struct task_trampoline_arg *arg = (struct task_trampoline_arg *)arg1;
	void *result;

	//printk("DEBUG: task_trampoline started, arg=%p\n", arg);
	k_msleep(10); /* Small delay to ensure parent is ready */

	if (!arg || !arg->run) {
		//printk("DEBUG: task_trampoline - invalid arg\n");
		return;
	}

	/* Execute user function */
	//printk("DEBUG: task_trampoline - calling user function at %p\n", arg->run);
	result = arg->run(arg->run_arg);
	//printk("DEBUG: task_trampoline - user function returned %p\n", result);

	/* Send exit notification */
	struct z_process *proc = process_current();
	//printk("DEBUG: task_trampoline - process_current returned %p\n", proc);
	if (proc) {
		//printk("DEBUG: task_trampoline - sending exit msg for PID %d\n", proc->pid);
		struct process_exit_msg msg = {
			.pid = proc->pid,
			.exit_code = (int)(intptr_t)result,
		};
		int ret = k_msgq_put(&process_exit_queue, &msg, K_NO_WAIT);
		//printk("DEBUG: task_trampoline - k_msgq_put returned %d\n", ret);
	}

	/* Free trampoline argument */
	//printk("DEBUG: task_trampoline - freeing trampoline arg\n");
	k_free(arg);

	/* CRITICAL: Yield to let waitpid start processing before we exit */
	//printk("DEBUG: task_trampoline - yielding before exit\n");
	k_yield();
	k_msleep(100);  /* Give plenty of time for waitpid to call k_thread_join */

	//printk("DEBUG: task_trampoline - about to return (thread will exit)\n");
	//printk("DEBUG: task_trampoline - SP=%p\n", (void *)__builtin_frame_address(0));
}

/**
 * @brief Create a new task/process
 *
 * Implementation based on Embox's new_task() in multi.c
 */
pid_t new_task(const char *name, void *(*run)(void *), void *arg)
{
	if (!run) {
		return -EINVAL;
	}

	//printk("DEBUG: new_task - getting parent process\n");
	/* Get current process as parent */
	struct z_process *parent = process_current();
	if (!parent) {
		//printk("DEBUG: new_task - no parent process!\n");
		return -ESRCH;
	}
	//printk("DEBUG: new_task - parent PID = %d\n", parent->pid);

	/* Create new process */
	//printk("DEBUG: new_task - creating child process\n");
	struct z_process *child = process_create(parent);
	if (!child) {
		//printk("DEBUG: new_task - process_create failed!\n");
		return -ENOMEM;
	}
	//printk("DEBUG: new_task - child PID = %d\n", child->pid);

	/* Allocate thread stack from pool */
	k_thread_stack_t *stack = NULL;
	int stack_index = -1;

	k_mutex_lock(&stack_pool_lock, K_FOREVER);
	for (int i = 0; i < MAX_STACK_POOL; i++) {
		if (!stack_pool_used[i]) {
			stack_pool_used[i] = true;
			stack = &stack_pool[i][0];  /* Direct array access */
			stack_index = i;
			//printk("DEBUG: new_task - allocated stack from pool index %d at %p\n", i, stack);
			break;
		}
	}
	k_mutex_unlock(&stack_pool_lock);

	if (!stack) {
		//printk("DEBUG: new_task - stack pool exhausted!\n");
		process_exit(child, -ENOMEM);
		return -ENOMEM;
	}

	/* Allocate thread structure */
	struct k_thread *thread = k_malloc(sizeof(struct k_thread));
	if (!thread) {
		k_mutex_lock(&stack_pool_lock, K_FOREVER);
		stack_pool_used[stack_index] = false;
		k_mutex_unlock(&stack_pool_lock);
		process_exit(child, -ENOMEM);
		return -ENOMEM;
	}

	/* Allocate trampoline argument */
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

	/* CRITICAL: Register thread with process BEFORE starting it
	 * Otherwise the thread starts immediately and gets the wrong process!
	 */
	//printk("DEBUG: new_task - registering thread with child process BEFORE create\n");
	process_register_thread(child, thread);

	/* Create and start thread */
	//printk("DEBUG: new_task - creating thread\n");
	k_tid_t tid = k_thread_create(
		thread,
		stack,
		K_THREAD_STACK_SIZEOF(TASK_STACK_SIZE),
		task_trampoline,
		tramp_arg,
		NULL,
		NULL,
		K_PRIO_PREEMPT(7),
		0,
		K_NO_WAIT
	);

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

	//printk("DEBUG: new_task - thread created, tid = %p\n", tid);

	/* Store stack index and thread pointers for later cleanup */
	k_mutex_lock(&stack_tracker_lock, K_FOREVER);
	for (int i = 0; i < MAX_TRACKED_STACKS; i++) {
		if (stack_tracker[i].pid == 0) {
			stack_tracker[i].pid = child->pid;
			stack_tracker[i].stack_index = stack_index;
			stack_tracker[i].thread = thread;
			/*printk("DEBUG: new_task - stored stack index %d for PID %d\n",
			  stack_index, child->pid);*/
			break;
		}
	}
	k_mutex_unlock(&stack_tracker_lock);

	/* Set thread name */
	if (name && name[0] != '\0') {
		k_thread_name_set(tid, name);
	} else {
		char default_name[16];
		snprintf(default_name, sizeof(default_name), "task_%d", child->pid);
		k_thread_name_set(tid, default_name);
	}

	//printk("DEBUG: new_task - returning PID %d\n", child->pid);
	return child->pid;
}

/**
 * @brief Wait for a process to complete
 *
 * Simplified waitpid for shell command execution
 */
pid_t waitpid(pid_t pid, int *status, int options)
{
	struct process_exit_msg msg;
	int ret;

	//printk("DEBUG: waitpid - waiting for PID %d\n", pid);

	/* Wait for exit message from the specified process */
	while (1) {
		ret = k_msgq_get(&process_exit_queue, &msg, K_FOREVER);
		if (ret == 0) {
			//printk("DEBUG: waitpid - got exit msg for PID %d\n", msg.pid);
			if (msg.pid == pid || pid == -1) {
				if (status) {
					*status = msg.exit_code;
				}

				/* CRITICAL: Wait for the thread to fully exit before cleanup */
				//printk("DEBUG: waitpid - getting process %d\n", msg.pid);
				struct z_process *proc = process_get(msg.pid);
				//printk("DEBUG: waitpid - process_get returned %p\n", proc);

				if (proc && proc->main_thread) {
					//printk("DEBUG: waitpid - main_thread = %p\n", proc->main_thread);

					/* Retrieve stack index from tracker */
					int stack_index = -1;
					struct k_thread *thread = proc->main_thread;

					k_mutex_lock(&stack_tracker_lock, K_FOREVER);
					for (int i = 0; i < MAX_TRACKED_STACKS; i++) {
						if (stack_tracker[i].pid == msg.pid) {
							stack_index = stack_tracker[i].stack_index;
							/*printk("DEBUG: waitpid - found stack index %d for PID %d\n",
							  stack_index, msg.pid);*/
							stack_tracker[i].pid = 0; /* Clear entry */
							stack_tracker[i].stack_index = -1;
							stack_tracker[i].thread = NULL;
							break;
						}
					}
					k_mutex_unlock(&stack_tracker_lock);

					//printk("DEBUG: waitpid - stack_index = %d\n", stack_index);

					/* Join the thread to ensure it's completely done */
					//printk("DEBUG: waitpid - calling k_thread_join...\n");
					k_thread_join(proc->main_thread, K_FOREVER);
					//printk("DEBUG: waitpid - k_thread_join completed\n");

					/* Unregister thread from process */
					//printk("DEBUG: waitpid - unregistering thread\n");
					process_unregister_thread(proc, thread);

					/* Free thread structure (but NOT the stack - it's static) */
					//printk("DEBUG: waitpid - freeing thread\n");
					k_free(thread);

					/* Return stack to pool */
					if (stack_index >= 0 && stack_index < MAX_STACK_POOL) {
						k_mutex_lock(&stack_pool_lock, K_FOREVER);
						stack_pool_used[stack_index] = false;
						k_mutex_unlock(&stack_pool_lock);
						//printk("DEBUG: waitpid - returned stack index %d to pool\n", stack_index);
					}

					/* Clean up process */
					//printk("DEBUG: waitpid - cleaning up process\n");
					process_exit(proc, msg.exit_code);
				}

				//printk("DEBUG: waitpid - returning %d\n", msg.pid);
				return msg.pid;
			}
			/* Not our process, put it back */
			k_msgq_put(&process_exit_queue, &msg, K_NO_WAIT);
			k_yield();
		}
	}

	return -ECHILD;
}

/**
 * @brief Command execution wrapper
 *
 * Similar to Embox's run_cmd() in tish.c
 */
static void *run_cmd(void *data)
{
	struct cmd_data *cdata_ptr = (struct cmd_data *)data;
	struct cmd_data cdata;
	int ret;

	if (!cdata_ptr || !cdata_ptr->cmd || !cdata_ptr->cmd->exec) {
		if (cdata_ptr) {
			k_sem_give(&cdata_ptr->copied_sem);
		}
		return (void *)(intptr_t)(-EINVAL);
	}

	/* Copy command data to our stack FIRST - CRITICAL! */
	memcpy(&cdata, cdata_ptr, sizeof(struct cmd_data));

	/* Signal parent that we've copied the data - USE SEMAPHORE */
	k_sem_give(&cdata_ptr->copied_sem);

	/* Execute command using our local copy */
	ret = cdata.cmd->exec(cdata.argc, cdata.argv);

	return (void *)(intptr_t)ret;
}

/**
 * @brief Execute a shell command in a new process
 *
 * Similar to Embox's process_external() in tish.c
 */
int shell_exec_command(const struct shell_cmd *cmd, int argc, char **argv, bool on_fg)
{
	struct cmd_data cdata = {
		.argc = argc,
		.argv = argv,
		.cmd = cmd,
		.on_fg = on_fg,
		.result = 0,
	};

	//printk("DEBUG: shell_exec_command - starting for '%s'\n", cmd->name);

	/* Initialize semaphore for synchronization */
	k_sem_init(&cdata.copied_sem, 0, 1);

	/* Create new task for command execution */
	//printk("DEBUG: shell_exec_command - calling new_task\n");
	pid_t pid = new_task(cmd->name, run_cmd, &cdata);
	//printk("DEBUG: shell_exec_command - new_task returned PID %d\n", pid);
	if (pid < 0) {
	  /*printk("Error: Failed to create task for command '%s': %d\n",
	    cmd->name, pid);*/
		return pid;
	}

	/* CRITICAL: Wait for child to copy cmd_data using semaphore
	 * This is more reliable than polling with k_yield or k_msleep
	 */
	int ret = k_sem_take(&cdata.copied_sem, K_MSEC(1000));
	if (ret != 0) {
		//printk("ERROR: Timeout waiting for child process %d to copy data\n", pid);
		return -ETIMEDOUT;
	}

	/* Now safe - child has copied the data, we can continue */

	/* Wait for foreground commands to complete */
	if (on_fg) {
		int status = 0;
		pid_t wait_result = waitpid(pid, &status, 0);
		if (wait_result < 0) {
			//printk("Error: waitpid failed: %d\n", wait_result);
			return wait_result;
		}
		return status;
	} else {
		/* Background command */
		//printk("[%d] %s &\n", pid, cmd->name);
		return 0;
	}
}

/**
 * @brief Find a command by name
 */
const struct shell_cmd *shell_cmd_lookup(const char *name)
{
	if (!name) {
		return NULL;
	}

	k_mutex_lock(&cmd_registry_lock, K_FOREVER);

	for (int i = 0; i < command_count; i++) {
		if (command_registry[i] &&
		    strcmp(command_registry[i]->name, name) == 0) {
			k_mutex_unlock(&cmd_registry_lock);
			return command_registry[i];
		}
	}

	k_mutex_unlock(&cmd_registry_lock);
	return NULL;
}

/**
 * @brief Register a shell command
 */
int shell_cmd_register(const struct shell_cmd *cmd)
{
	if (!cmd || !cmd->name || !cmd->exec) {
		return -EINVAL;
	}

	k_mutex_lock(&cmd_registry_lock, K_FOREVER);

	if (command_count >= MAX_COMMANDS) {
		k_mutex_unlock(&cmd_registry_lock);
		return -ENOMEM;
	}

	command_registry[command_count++] = cmd;
	k_mutex_unlock(&cmd_registry_lock);

	return 0;
}

/**
 * @brief Initialize shell process subsystem
 */
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

/* Initialize at startup */
static int shell_process_init_wrapper(void)
{
	shell_process_init();
	return 0;
}

SYS_INIT(shell_process_init_wrapper, APPLICATION, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT);
