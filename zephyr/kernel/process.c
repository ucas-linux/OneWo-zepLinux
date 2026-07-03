/*
 * Copyright (c) 2024 OneWo-rtLinux Team
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/kernel/process.h>
#include <zephyr/init.h>
#include <string.h>
#include <errno.h>

/* Default values if not configured */
#ifndef CONFIG_MAX_PROCESS_COUNT
#define CONFIG_MAX_PROCESS_COUNT 16
#endif

#ifndef CONFIG_MAX_FD_PER_PROCESS
#define CONFIG_MAX_FD_PER_PROCESS 16
#endif

/* Helper function to duplicate string */
static char *z_strdup(const char *str)
{
	if (!str) {
		return NULL;
	}

	size_t len = strlen(str) + 1;
	char *dup = k_malloc(len);
	if (dup) {
		memcpy(dup, str, len);
	}
	return dup;
}

/* Process table - static allocation for MCU */
static struct z_process process_table[CONFIG_MAX_PROCESS_COUNT];
static uint32_t process_allocated_mask;
static pid_t next_pid = PID_INIT;

/* Lock for process table operations */
static struct k_spinlock process_lock = {};

/**
 * @brief Allocate a PID
 */
static pid_t alloc_pid(void)
{
	k_spinlock_key_t key = k_spin_lock(&process_lock);

	for (int i = 0; i < CONFIG_MAX_PROCESS_COUNT; i++) {
		if (!(process_allocated_mask & BIT(i))) {
			process_allocated_mask |= BIT(i);
			pid_t pid = next_pid++;
			k_spin_unlock(&process_lock, key);
			return pid;
		}
	}

	k_spin_unlock(&process_lock, key);
	return PID_INVALID;
}

/**
 * @brief Free a PID
 */
static void free_pid(pid_t pid)
{
	k_spinlock_key_t key = k_spin_lock(&process_lock);

	for (int i = 0; i < CONFIG_MAX_PROCESS_COUNT; i++) {
		if (process_table[i].pid == pid) {
			process_allocated_mask &= ~BIT(i);
			break;
		}
	}

	k_spin_unlock(&process_lock, key);
}

/**
 * @brief Find free process slot
 */
static struct z_process *alloc_process_slot(void)
{
	k_spinlock_key_t key = k_spin_lock(&process_lock);

	for (int i = 0; i < CONFIG_MAX_PROCESS_COUNT; i++) {
		if (!(process_allocated_mask & BIT(i))) {
			k_spin_unlock(&process_lock, key);
			return &process_table[i];
		}
	}

	k_spin_unlock(&process_lock, key);
	return NULL;
}

void z_process_init(void)
{
	memset(process_table, 0, sizeof(process_table));
	process_allocated_mask = 0;
	next_pid = PID_INIT;

	/* Initialize init process (PID 1) */
	struct z_process *init_proc = process_create(NULL);
	if (init_proc) {
		init_proc->pid = PID_INIT;
		next_pid = PID_INIT + 1;
	}
}

/* Wrapper for SYS_INIT */
static int z_process_init_wrapper(void)
{
	z_process_init();
	return 0;
}

struct z_process *process_create(struct z_process *parent)
{
	struct z_process *proc = alloc_process_slot();
	if (!proc) {
		return NULL;
	}

	pid_t pid = alloc_pid();
	if (pid == PID_INVALID) {
		return NULL;
	}

	/* Initialize process structure */
	memset(proc, 0, sizeof(struct z_process));
	proc->pid = pid;
	proc->parent = parent;
	proc->main_thread = NULL;

	sys_dlist_init(&proc->children);
	sys_dlist_init(&proc->env_list);
	sys_dlist_init(&proc->threads);

	atomic_set(&proc->ref_count, 1);

	/* Initialize file descriptor table */
	memset(&proc->fd_table, 0, sizeof(struct idesc_table));

	/* Add to parent's child list */
	if (parent) {
		k_spinlock_key_t key = k_spin_lock(&process_lock);
		sys_dlist_append(&parent->children, &proc->child_node);
		k_spin_unlock(&process_lock, key);
	}

	return proc;
}

struct z_process *process_get(pid_t pid)
{
	if (pid == PID_INVALID) {
		return NULL;
	}

	k_spinlock_key_t key = k_spin_lock(&process_lock);

	for (int i = 0; i < CONFIG_MAX_PROCESS_COUNT; i++) {
		if ((process_allocated_mask & BIT(i)) &&
		    process_table[i].pid == pid) {
			k_spin_unlock(&process_lock, key);
			return &process_table[i];
		}
	}

	k_spin_unlock(&process_lock, key);
	return NULL;
}

struct z_process *process_current(void)
{
	struct k_thread *thread = k_current_get();
	if (!thread) {
		return NULL;
	}

	/* Return thread's associated process */
	if (thread->process) {
		/* Validate that process pointer is within process_table bounds */
		uintptr_t proc_addr = (uintptr_t)thread->process;
		uintptr_t table_start = (uintptr_t)&process_table[0];
		uintptr_t table_end = (uintptr_t)&process_table[CONFIG_MAX_PROCESS_COUNT];

		if (proc_addr < table_start || proc_addr >= table_end) {
			/* Pointer outside process table, clear it */
			thread->process = NULL;
		} else {
			/* Pointer is within table, safe to access */
			/* Validate it's still allocated and matches table entry */
			size_t index = (proc_addr - table_start) / sizeof(struct z_process);
			if (index < CONFIG_MAX_PROCESS_COUNT &&
			    (process_allocated_mask & BIT(index)) &&
			    thread->process == &process_table[index]) {
				return thread->process;
			}
			/* Process pointer is stale or misaligned, clear it */
			thread->process = NULL;
		}
	}

	/* Fallback to init process if no process assigned */
	return process_get(PID_INIT);
}

void process_exit(struct z_process *proc, int exit_code)
{
	if (!proc) {
		return;
	}

	proc->exit_code = exit_code;

	k_spinlock_key_t key = k_spin_lock(&process_lock);

	/* Remove from parent's child list */
	if (proc->parent) {
		sys_dlist_remove(&proc->child_node);
		proc->parent = NULL;  /* Clear parent pointer */
	}

	/* Free environment variables */
	sys_dnode_t *node, *next;
	SYS_DLIST_FOR_EACH_NODE_SAFE(&proc->env_list, node, next) {
		struct env_entry *entry = CONTAINER_OF(node, struct env_entry, node);
		sys_dlist_remove(node);
		if (entry->key) {
			k_free(entry->key);
		}
		if (entry->value) {
			k_free(entry->value);
		}
		k_free(entry);
	}

	/* Clear file descriptor table */
	memset(&proc->fd_table, 0, sizeof(struct idesc_table));

	/* Free PID */
	free_pid(proc->pid);

	/* Mark process as invalid */
	proc->pid = PID_INVALID;

	k_spin_unlock(&process_lock, key);
}

void *process_idesc_table_get(struct z_process *proc, int fd)
{
	if (!proc || fd < 0 || fd >= CONFIG_MAX_FD_PER_PROCESS) {
		return NULL;
	}

	if (!(proc->fd_table.allocated_mask & BIT(fd))) {
		return NULL;
	}

	return proc->fd_table.entries[fd].idesc;
}

int process_idesc_table_add(struct z_process *proc, void *idesc)
{
	if (!proc || !idesc) {
		return -EINVAL;
	}

	k_spinlock_key_t key = k_spin_lock(&process_lock);

	/* Find first free FD */
	for (int fd = 0; fd < CONFIG_MAX_FD_PER_PROCESS; fd++) {
		if (!(proc->fd_table.allocated_mask & BIT(fd))) {
			proc->fd_table.entries[fd].idesc = idesc;
			proc->fd_table.entries[fd].flags = 0;
			proc->fd_table.allocated_mask |= BIT(fd);
			k_spin_unlock(&process_lock, key);
			return fd;
		}
	}

	k_spin_unlock(&process_lock, key);
	return -EMFILE;  /* Too many open files */
}

int process_idesc_table_remove(struct z_process *proc, int fd)
{
	if (!proc || fd < 0 || fd >= CONFIG_MAX_FD_PER_PROCESS) {
		return -EINVAL;
	}

	k_spinlock_key_t key = k_spin_lock(&process_lock);

	if (!(proc->fd_table.allocated_mask & BIT(fd))) {
		k_spin_unlock(&process_lock, key);
		return -EBADF;  /* Bad file descriptor */
	}

	proc->fd_table.entries[fd].idesc = NULL;
	proc->fd_table.entries[fd].flags = 0;
	proc->fd_table.allocated_mask &= ~BIT(fd);

	k_spin_unlock(&process_lock, key);
	return 0;
}

const char *process_getenv(struct z_process *proc, const char *name)
{
	if (!proc || !name) {
		return NULL;
	}

	k_spinlock_key_t key = k_spin_lock(&process_lock);

	sys_dnode_t *node;
	SYS_DLIST_FOR_EACH_NODE(&proc->env_list, node) {
		struct env_entry *entry = CONTAINER_OF(node, struct env_entry, node);
		if (entry->key && strcmp(entry->key, name) == 0) {
			const char *value = entry->value;
			k_spin_unlock(&process_lock, key);
			return value;
		}
	}

	k_spin_unlock(&process_lock, key);
	return NULL;
}

int process_setenv(struct z_process *proc, const char *name, const char *value)
{
	if (!proc || !name) {
		return -EINVAL;
	}

	k_spinlock_key_t key = k_spin_lock(&process_lock);

	/* Check if variable already exists */
	sys_dnode_t *node;
	SYS_DLIST_FOR_EACH_NODE(&proc->env_list, node) {
		struct env_entry *entry = CONTAINER_OF(node, struct env_entry, node);
		if (entry->key && strcmp(entry->key, name) == 0) {
			/* Update existing entry */
			if (entry->value) {
				k_free(entry->value);
			}
			entry->value = value ? z_strdup(value) : NULL;
			k_spin_unlock(&process_lock, key);
			return 0;
		}
	}

	/* Create new entry */
	struct env_entry *entry = k_malloc(sizeof(struct env_entry));
	if (!entry) {
		k_spin_unlock(&process_lock, key);
		return -ENOMEM;
	}

	entry->key = z_strdup(name);
	entry->value = value ? z_strdup(value) : NULL;

	if (!entry->key || (value && !entry->value)) {
		if (entry->key) k_free(entry->key);
		if (entry->value) k_free(entry->value);
		k_free(entry);
		k_spin_unlock(&process_lock, key);
		return -ENOMEM;
	}

	sys_dlist_append(&proc->env_list, &entry->node);
	k_spin_unlock(&process_lock, key);

	return 0;
}

struct z_process *process_fork(struct z_process *parent)
{
	if (!parent) {
		return NULL;
	}

	struct z_process *child = process_create(parent);
	if (!child) {
		return NULL;
	}

	k_spinlock_key_t key = k_spin_lock(&process_lock);

	/* Copy file descriptor table */
	memcpy(&child->fd_table, &parent->fd_table, sizeof(struct idesc_table));

	/* Copy environment variables */
	sys_dnode_t *node;
	SYS_DLIST_FOR_EACH_NODE(&parent->env_list, node) {
		struct env_entry *parent_entry = CONTAINER_OF(node, struct env_entry, node);

		struct env_entry *child_entry = k_malloc(sizeof(struct env_entry));
		if (!child_entry) {
			continue;
		}

		child_entry->key = z_strdup(parent_entry->key);
		child_entry->value = parent_entry->value ? z_strdup(parent_entry->value) : NULL;

		if (!child_entry->key || (parent_entry->value && !child_entry->value)) {
			if (child_entry->key) k_free(child_entry->key);
			if (child_entry->value) k_free(child_entry->value);
			k_free(child_entry);
			continue;
		}

		sys_dlist_append(&child->env_list, &child_entry->node);
	}

	k_spin_unlock(&process_lock, key);

	return child;
}

int process_register_thread(struct z_process *proc, struct k_thread *thread)
{
	if (!proc || !thread) {
		return -EINVAL;
	}

	k_spinlock_key_t key = k_spin_lock(&process_lock);

	/* Set thread's process pointer */
	thread->process = proc;

	/* Add thread to process's thread list */
	sys_dlist_append(&proc->threads, &thread->process_thread_node);

	/* If this is the first thread, set it as main thread */
	if (!proc->main_thread) {
		proc->main_thread = thread;
	}

	k_spin_unlock(&process_lock, key);

	return 0;
}

int process_unregister_thread(struct z_process *proc, struct k_thread *thread)
{
	if (!proc || !thread) {
		return -EINVAL;
	}

	k_spinlock_key_t key = k_spin_lock(&process_lock);

	/* Remove from thread list */
	sys_dlist_remove(&thread->process_thread_node);

	/* Clear process pointer */
	thread->process = NULL;

	/* If this was the main thread, clear it */
	if (proc->main_thread == thread) {
		proc->main_thread = NULL;
	}

	k_spin_unlock(&process_lock, key);

	return 0;
}

/* Initialize process subsystem at kernel init */
SYS_INIT(z_process_init_wrapper, POST_KERNEL, CONFIG_KERNEL_INIT_PRIORITY_DEFAULT);
