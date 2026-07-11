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
#include <zephyr/console/console.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
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

/* Bytecode VM integration */
#include "bytecode_vm.h"

/**
 * @brief Shell command: ls - List available programs
 */
static int cmd_ls_exec(int argc, char **argv)
{
	printk("Built-in commands:\n");
	k_mutex_lock(&cmd_registry_lock, K_FOREVER);
	for (int i = 0; i < command_count; i++) {
		if (command_registry[i]) {
			printk("  %-16s  (builtin)\n", command_registry[i]->name);
		}
	}
	k_mutex_unlock(&cmd_registry_lock);

	printk("\n");
	vm_list_programs();

	return 0;
}

static const struct shell_cmd cmd_ls = {
	.name = "ls",
	.exec = cmd_ls_exec,
	.brief = "List available programs"
};

/**
 * @brief Shell command: upload - Upload bytecode program (simulated)
 * Usage: upload <name> <program_id>
 */
static int cmd_upload_exec(int argc, char **argv)
{
	if (argc < 3) {
		printk("Usage: upload <name> <program_id>\n");
		printk("Available program_id:\n");
		printk("  hello    - Hello world program\n");
		printk("  counter  - Count from 1 to 10\n");
		printk("  calc     - Simple calculator demo\n");
		return -EINVAL;
	}

	const char *name = argv[1];
	const char *prog_id = argv[2];

	/* Sample bytecode programs (for testing) */

	/* Program: hello - prints "Hello World!" and number 42 */
	static const uint8_t prog_hello[] = {
		OP_PRINT_STR, 13, 'H', 'e', 'l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', '!', '\n',
		OP_PUSH, 0, 0, 0, 42,    // Push 42
		OP_PRINT,                // Print it
		OP_HALT
	};

	/* Program: counter - counts from 1 to 10 */
	static const uint8_t prog_counter[] = {
		// Initialize counter to 1
		OP_PUSH, 0, 0, 0, 1,     // counter = 1

		// Loop start (PC = 5)
		OP_DUP,                  // Duplicate counter for comparison
		OP_PUSH, 0, 0, 0, 11,    // Push 11
		OP_LT,                   // counter < 11 ?
		OP_JZ, 0, 0, 0, 33,      // If false, jump to end (PC 33)

		// Loop body
		OP_DUP,                  // Duplicate counter for printing
		OP_PRINT,                // Print counter
		OP_PUSH, 0, 0, 0, 1,     // Push 1
		OP_ADD,                  // counter++
		OP_PUSH, 0, 0, 0, 100,   // Push 100ms
		OP_SLEEP,                // Sleep
		OP_JMP, 0, 0, 0, 5,      // Jump back to loop start

		// End (PC = 33)
		OP_POP,                  // Clean up stack
		OP_HALT
	};

	/* Program: calc - demonstrates arithmetic (5 + 3) * 2 = 16 */
	static const uint8_t prog_calc[] = {
		OP_PRINT_STR, 17, 'C', 'a', 'l', 'c', ':', ' ', '(', '5', '+', '3', ')', '*', '2', ' ', '=', ' ','\n',
		OP_PUSH, 0, 0, 0, 5,     // Push 5
		OP_PUSH, 0, 0, 0, 3,     // Push 3
		OP_ADD,                  // 5 + 3 = 8
		OP_PUSH, 0, 0, 0, 2,     // Push 2
		OP_MUL,                  // 8 * 2 = 16
		OP_PRINT,                // Print result
		OP_HALT
	};

	const uint8_t *code = NULL;
	size_t code_size = 0;

	if (strcmp(prog_id, "hello") == 0) {
		code = prog_hello;
		code_size = sizeof(prog_hello);
	} else if (strcmp(prog_id, "counter") == 0) {
		code = prog_counter;
		code_size = sizeof(prog_counter);
	} else if (strcmp(prog_id, "calc") == 0) {
		code = prog_calc;
		code_size = sizeof(prog_calc);
	} else {
		printk("Unknown program_id: %s\n", prog_id);
		return -EINVAL;
	}

	printk("Uploading program '%s' (%zu bytes)...\n", name, code_size);
	int ret = vm_load_program(name, code, code_size);
	if (ret < 0) {
		printk("Failed to load program: %d\n", ret);
		return ret;
	}

	printk("Upload complete. Use 'run %s' to execute.\n", name);
	return 0;
}

static const struct shell_cmd cmd_upload = {
	.name = "upload",
	.exec = cmd_upload_exec,
	.brief = "Upload bytecode program"
};

/**
 * @brief Shell command: run - Execute bytecode program
 * Usage: run <name>
 */
static int cmd_run_exec(int argc, char **argv)
{
	if (argc < 2) {
		printk("Usage: run <program_name>\n");
		return -EINVAL;
	}

	const char *name = argv[1];
	return vm_execute_program(name, argc - 1, &argv[1]);
}

static const struct shell_cmd cmd_run = {
	.name = "run",
	.exec = cmd_run_exec,
	.brief = "Execute bytecode program"
};

/**
 * @brief Shell command: rm - Delete bytecode program
 * Usage: rm <name>
 */
static int cmd_rm_exec(int argc, char **argv)
{
	if (argc < 2) {
		printk("Usage: rm <program_name>\n");
		return -EINVAL;
	}

	const char *name = argv[1];
	return vm_delete_program(name);
}

static const struct shell_cmd cmd_rm = {
	.name = "rm",
	.exec = cmd_rm_exec,
	.brief = "Delete bytecode program"
};

/**
 * @brief Helper: Parse hex digit
 */
static int hex_to_nibble(char c)
{
	if (c >= '0' && c <= '9') return c - '0';
	if (c >= 'a' && c <= 'f') return c - 'a' + 10;
	if (c >= 'A' && c <= 'F') return c - 'A' + 10;
	return -1;
}

/**
 * @brief Helper: Parse hex byte
 */
static int parse_hex_byte(const char *str)
{
	int high = hex_to_nibble(str[0]);
	int low = hex_to_nibble(str[1]);

	if (high < 0 || low < 0) {
		return -1;
	}

	return (high << 4) | low;
}

/**
 * @brief Shell command: upload_hex - Upload bytecode from hex string
 * Usage: upload_hex <name> <hex_string>
 *
 * Example: upload_hex test "01 00 00 00 2A 40 FF"
 * This uploads a program that pushes 42, prints it, and halts.
 */
static int cmd_upload_hex_exec(int argc, char **argv)
{
	if (argc < 3) {
		printk("Usage: upload_hex <name> <hex_string>\n");
		printk("\n");
		printk("Upload bytecode from a hex string.\n");
		printk("Hex bytes should be space-separated.\n");
		printk("\n");
		printk("Example:\n");
		printk("  upload_hex test \"01 00 00 00 2A 40 FF\"\n");
		printk("\n");
		printk("This creates a program that:\n");
		printk("  01 00 00 00 2A  - PUSH 42\n");
		printk("  40              - PRINT\n");
		printk("  FF              - HALT\n");
		printk("\n");
		printk("Instruction reference:\n");
		printk("  01 <4-byte val> - PUSH value\n");
		printk("  02              - POP\n");
		printk("  03              - DUP\n");
		printk("  10              - ADD\n");
		printk("  11              - SUB\n");
		printk("  12              - MUL\n");
		printk("  13              - DIV\n");
		printk("  40              - PRINT (print top of stack)\n");
		printk("  41 <len> <str>  - PRINT_STR\n");
		printk("  42              - SLEEP (ms from stack)\n");
		printk("  FF              - HALT\n");
		return -EINVAL;
	}

	const char *name = argv[1];
	const char *hex_str = argv[2];

	/* Count expected bytes (rough estimate) */
	int hex_len = strlen(hex_str);
	int max_bytes = (hex_len / 2) + 1;

	if (max_bytes > VM_MAX_PROGRAM_SIZE) {
		printk("Hex string too long (max %d bytes)\n", VM_MAX_PROGRAM_SIZE);
		return -EINVAL;
	}

	/* Allocate buffer for bytecode */
	uint8_t *bytecode = k_malloc(max_bytes);
	if (!bytecode) {
		printk("Failed to allocate memory for bytecode\n");
		return -ENOMEM;
	}

	/* Parse hex string */
	const char *ptr = hex_str;
	int byte_count = 0;

	while (*ptr != '\0' && byte_count < max_bytes) {
		/* Skip whitespace */
		while (*ptr == ' ' || *ptr == '\t' || *ptr == '\r' || *ptr == '\n') {
			ptr++;
		}

		if (*ptr == '\0') {
			break;
		}

		/* Need at least 2 hex digits */
		if (!ptr[1]) {
			printk("ERROR: Incomplete hex byte at position %d\n",
			       (int)(ptr - hex_str));
			k_free(bytecode);
			return -EINVAL;
		}

		/* Parse hex byte */
		int byte_val = parse_hex_byte(ptr);
		if (byte_val < 0) {
			printk("ERROR: Invalid hex byte at position %d: '%c%c'\n",
			       (int)(ptr - hex_str), ptr[0], ptr[1]);
			k_free(bytecode);
			return -EINVAL;
		}

		bytecode[byte_count++] = (uint8_t)byte_val;
		ptr += 2;
	}

	if (byte_count == 0) {
		printk("ERROR: No bytecode parsed\n");
		k_free(bytecode);
		return -EINVAL;
	}

	printk("Parsed %d bytes from hex string\n", byte_count);

	/* Load program into VM */
	int ret = vm_load_program(name, bytecode, byte_count);

	/* Free temporary buffer */
	k_free(bytecode);

	if (ret < 0) {
		printk("Failed to load program: %d\n", ret);
		return ret;
	}

	printk("Program '%s' loaded successfully!\n", name);
	printk("Use 'run %s' to execute.\n", name);

	return 0;
}

static const struct shell_cmd cmd_upload_hex = {
	.name = "upload_hex",
	.exec = cmd_upload_hex_exec,
	.brief = "Upload bytecode from hex string"
};

/**
 * @brief Register bytecode commands
 */
static int register_bytecode_commands(void)
{
	shell_cmd_register(&cmd_ls);
	shell_cmd_register(&cmd_upload);
	shell_cmd_register(&cmd_upload_hex);
	shell_cmd_register(&cmd_run);
	shell_cmd_register(&cmd_rm);
	return 0;
}

SYS_INIT(register_bytecode_commands, APPLICATION, 99);
