/*
 * Copyright (c) 2024 OneWo-rtLinux Team
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_INCLUDE_KERNEL_PROCESS_H_
#define ZEPHYR_INCLUDE_KERNEL_PROCESS_H_

#include <zephyr/sys/dlist.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/atomic.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Process ID type
 */
typedef int32_t pid_t;

/* Special PID values */
#define PID_INVALID 0
#define PID_INIT 1

/* Maximum number of processes (MCU constraint) */
#ifndef CONFIG_MAX_PROCESS_COUNT
#define CONFIG_MAX_PROCESS_COUNT 16
#endif

/* Maximum file descriptors per process */
#ifndef CONFIG_MAX_FD_PER_PROCESS
#define CONFIG_MAX_FD_PER_PROCESS 16
#endif

/* Forward declarations */
struct z_process;
struct k_thread;

/**
 * @brief File descriptor table entry
 */
struct idesc_entry {
	void *idesc;           /* Pointer to file descriptor object */
	uint32_t flags;        /* Flags (CLOEXEC in low bit) */
};

/**
 * @brief File descriptor table
 *
 * Fixed-size array for MCU environments
 */
struct idesc_table {
	struct idesc_entry entries[CONFIG_MAX_FD_PER_PROCESS];
	uint32_t allocated_mask;  /* Bitmap of allocated FDs */
};

/**
 * @brief Environment variable entry
 */
struct env_entry {
	sys_dnode_t node;
	char *key;
	char *value;
};

/**
 * @brief Process structure
 *
 * Adapted from Embox's struct task for Zephyr RTOS.
 * Provides Linux-like process abstraction on top of k_thread.
 */
struct z_process {
	pid_t pid;                        /* Process ID */
	struct z_process *parent;         /* Parent process */
	struct k_thread *main_thread;     /* Main thread of this process */

	sys_dlist_t children;             /* List of child processes */
	sys_dnode_t child_node;           /* Node in parent's children list */

	sys_dlist_t threads;              /* List of threads in this process */

	/* Process resources */
	struct idesc_table fd_table;      /* File descriptor table */
	sys_dlist_t env_list;             /* Environment variables */

	/* Process state */
	atomic_t ref_count;               /* Reference count */
	uint32_t flags;                   /* Process flags */
	int exit_code;                    /* Exit code when terminated */
};

/**
 * @brief Initialize the process subsystem
 */
void z_process_init(void);

/**
 * @brief Create a new process
 *
 * @param parent Parent process (NULL for init process)
 * @return Pointer to new process or NULL on failure
 */
struct z_process *process_create(struct z_process *parent);

/**
 * @brief Get process by PID
 *
 * @param pid Process ID
 * @return Pointer to process or NULL if not found
 */
struct z_process *process_get(pid_t pid);

/**
 * @brief Get current process
 *
 * @return Pointer to current thread's process or NULL
 */
struct z_process *process_current(void);

/**
 * @brief Terminate a process
 *
 * @param proc Process to terminate
 * @param exit_code Exit code
 */
void process_exit(struct z_process *proc, int exit_code);

/**
 * @brief Get file descriptor from process table
 *
 * @param proc Process
 * @param fd File descriptor number
 * @return Pointer to descriptor object or NULL
 */
void *process_idesc_table_get(struct z_process *proc, int fd);

/**
 * @brief Allocate a file descriptor
 *
 * @param proc Process
 * @param idesc Descriptor object to store
 * @return File descriptor number or negative error code
 */
int process_idesc_table_add(struct z_process *proc, void *idesc);

/**
 * @brief Free a file descriptor
 *
 * @param proc Process
 * @param fd File descriptor to free
 * @return 0 on success, negative error code on failure
 */
int process_idesc_table_remove(struct z_process *proc, int fd);

/**
 * @brief Get environment variable
 *
 * @param proc Process
 * @param name Variable name
 * @return Variable value or NULL if not found
 */
const char *process_getenv(struct z_process *proc, const char *name);

/**
 * @brief Set environment variable
 *
 * @param proc Process
 * @param name Variable name
 * @param value Variable value
 * @return 0 on success, negative error code on failure
 */
int process_setenv(struct z_process *proc, const char *name, const char *value);

/**
 * @brief Fork a process (copy resources)
 *
 * @param parent Parent process to fork from
 * @return Pointer to new child process or NULL on failure
 */
struct z_process *process_fork(struct z_process *parent);

/**
 * @brief Register a thread with a process
 *
 * @param proc Process
 * @param thread Thread to register
 * @return 0 on success, negative error code on failure
 */
int process_register_thread(struct z_process *proc, struct k_thread *thread);

/**
 * @brief Unregister a thread from a process
 *
 * @param proc Process
 * @param thread Thread to unregister
 * @return 0 on success, negative error code on failure
 */
int process_unregister_thread(struct z_process *proc, struct k_thread *thread);

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_INCLUDE_KERNEL_PROCESS_H_ */
