/*
 * Copyright (c) 2024 OneWo-rtLinux Team
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/ztest.h>
#include <zephyr/kernel.h>
#include <zephyr/kernel/process.h>
#include <zephyr/posix/unistd.h>
#include <string.h>

/**
 * @brief Test process creation and PID allocation
 */
ZTEST(process_management, test_process_create)
{
	struct z_process *proc1, *proc2;

	/* Create first process */
	proc1 = process_create(NULL);
	zassert_not_null(proc1, "Failed to create process 1");
	zassert_true(proc1->pid > 0, "Invalid PID: %d", proc1->pid);

	/* Create second process */
	proc2 = process_create(NULL);
	zassert_not_null(proc2, "Failed to create process 2");
	zassert_true(proc2->pid > 0, "Invalid PID: %d", proc2->pid);

	/* PIDs should be different */
	zassert_not_equal(proc1->pid, proc2->pid, "PIDs should be unique");

	/* Clean up */
	process_exit(proc1, 0);
	process_exit(proc2, 0);
}

/**
 * @brief Test parent-child process relationship
 */
ZTEST(process_management, test_process_parent_child)
{
	struct z_process *parent, *child;

	/* Create parent process */
	parent = process_create(NULL);
	zassert_not_null(parent, "Failed to create parent process");

	/* Create child process */
	child = process_create(parent);
	zassert_not_null(child, "Failed to create child process");

	/* Verify parent-child relationship */
	zassert_equal(child->parent, parent, "Child's parent pointer incorrect");

	/* Clean up */
	process_exit(child, 0);
	process_exit(parent, 0);
}

/**
 * @brief Test process_get by PID
 */
ZTEST(process_management, test_process_get)
{
	struct z_process *proc, *retrieved;
	pid_t pid;

	/* Create a process */
	proc = process_create(NULL);
	zassert_not_null(proc, "Failed to create process");
	pid = proc->pid;

	/* Retrieve process by PID */
	retrieved = process_get(pid);
	zassert_equal(retrieved, proc, "Retrieved wrong process");

	/* Try to get invalid PID */
	retrieved = process_get(PID_INVALID);
	zassert_is_null(retrieved, "Should return NULL for invalid PID");

	/* Clean up */
	process_exit(proc, 0);
}

/**
 * @brief Test file descriptor table operations
 */
ZTEST(process_management, test_process_fd_table)
{
	struct z_process *proc;
	void *dummy_fd1 = (void *)0x1234;
	void *dummy_fd2 = (void *)0x5678;
	int fd1, fd2;
	void *retrieved;

	/* Create process */
	proc = process_create(NULL);
	zassert_not_null(proc, "Failed to create process");

	/* Add first file descriptor */
	fd1 = process_idesc_table_add(proc, dummy_fd1);
	zassert_true(fd1 >= 0, "Failed to add fd1: %d", fd1);

	/* Add second file descriptor */
	fd2 = process_idesc_table_add(proc, dummy_fd2);
	zassert_true(fd2 >= 0, "Failed to add fd2: %d", fd2);

	/* FDs should be different */
	zassert_not_equal(fd1, fd2, "FDs should be unique");

	/* Retrieve file descriptors */
	retrieved = process_idesc_table_get(proc, fd1);
	zassert_equal(retrieved, dummy_fd1, "Retrieved wrong fd1");

	retrieved = process_idesc_table_get(proc, fd2);
	zassert_equal(retrieved, dummy_fd2, "Retrieved wrong fd2");

	/* Remove fd1 */
	int ret = process_idesc_table_remove(proc, fd1);
	zassert_equal(ret, 0, "Failed to remove fd1");

	/* fd1 should now be invalid */
	retrieved = process_idesc_table_get(proc, fd1);
	zassert_is_null(retrieved, "fd1 should be invalid after removal");

	/* fd2 should still be valid */
	retrieved = process_idesc_table_get(proc, fd2);
	zassert_equal(retrieved, dummy_fd2, "fd2 should still be valid");

	/* Clean up */
	process_exit(proc, 0);
}

/**
 * @brief Test environment variables
 */
ZTEST(process_management, test_process_env)
{
	struct z_process *proc;
	const char *value;
	int ret;

	/* Create process */
	proc = process_create(NULL);
	zassert_not_null(proc, "Failed to create process");

	/* Set environment variable */
	ret = process_setenv(proc, "TEST_VAR", "test_value");
	zassert_equal(ret, 0, "Failed to set env variable");

	/* Get environment variable */
	value = process_getenv(proc, "TEST_VAR");
	zassert_not_null(value, "Failed to get env variable");
	zassert_equal(strcmp(value, "test_value"), 0, "Wrong env value");

	/* Update environment variable */
	ret = process_setenv(proc, "TEST_VAR", "new_value");
	zassert_equal(ret, 0, "Failed to update env variable");

	value = process_getenv(proc, "TEST_VAR");
	zassert_not_null(value, "Failed to get updated env variable");
	zassert_equal(strcmp(value, "new_value"), 0, "Wrong updated value");

	/* Get non-existent variable */
	value = process_getenv(proc, "NONEXISTENT");
	zassert_is_null(value, "Should return NULL for non-existent var");

	/* Clean up */
	process_exit(proc, 0);
}

/**
 * @brief Test process fork
 */
ZTEST(process_management, test_process_fork)
{
	struct z_process *parent, *child;
	const char *value;
	int ret;

	/* Create parent process */
	parent = process_create(NULL);
	zassert_not_null(parent, "Failed to create parent process");

	/* Set environment in parent */
	ret = process_setenv(parent, "PARENT_VAR", "parent_value");
	zassert_equal(ret, 0, "Failed to set parent env");

	/* Fork process */
	child = process_fork(parent);
	zassert_not_null(child, "Failed to fork process");

	/* Verify parent-child relationship */
	zassert_equal(child->parent, parent, "Fork child has wrong parent");

	/* Verify environment was copied */
	value = process_getenv(child, "PARENT_VAR");
	zassert_not_null(value, "Child should inherit parent's env");
	zassert_equal(strcmp(value, "parent_value"), 0, "Wrong inherited value");

	/* Modify child's environment shouldn't affect parent */
	ret = process_setenv(child, "PARENT_VAR", "child_value");
	zassert_equal(ret, 0, "Failed to update child env");

	value = process_getenv(parent, "PARENT_VAR");
	zassert_equal(strcmp(value, "parent_value"), 0,
	              "Parent env should not be affected by child");

	/* Clean up */
	process_exit(child, 0);
	process_exit(parent, 0);
}

/**
 * @brief Test thread registration with process
 */
ZTEST(process_management, test_thread_registration)
{
	struct z_process *proc;
	struct k_thread thread;
	k_thread_stack_t *stack;
	int ret;

	/* Allocate stack */
	stack = k_thread_stack_alloc(1024, 0);
	zassert_not_null(stack, "Failed to allocate stack");

	/* Create process */
	proc = process_create(NULL);
	zassert_not_null(proc, "Failed to create process");

	/* Initialize thread (don't start it) */
	memset(&thread, 0, sizeof(thread));

	/* Register thread with process */
	ret = process_register_thread(proc, &thread);
	zassert_equal(ret, 0, "Failed to register thread");

	/* Verify thread is associated with process */
	zassert_equal(thread.process, proc, "Thread not associated with process");

	/* Verify this is the main thread */
	zassert_equal(proc->main_thread, &thread, "Thread should be main thread");

	/* Unregister thread */
	ret = process_unregister_thread(proc, &thread);
	zassert_equal(ret, 0, "Failed to unregister thread");

	/* Verify thread is no longer associated */
	zassert_is_null(thread.process, "Thread should not be associated");

	/* Clean up */
	process_exit(proc, 0);
	k_thread_stack_free(stack);
}

/**
 * @brief Test getpid() returns correct PID
 */
ZTEST(process_management, test_getpid)
{
	struct z_process *proc;
	struct k_thread *current;
	pid_t pid;

	/* Get current thread */
	current = k_current_get();
	zassert_not_null(current, "No current thread");

	/* Create a process and register current thread */
	proc = process_create(NULL);
	zassert_not_null(proc, "Failed to create process");

	process_register_thread(proc, current);

	/* Call getpid() */
	pid = getpid();
	zassert_equal(pid, proc->pid, "getpid() returned wrong PID");

	/* Unregister and clean up */
	process_unregister_thread(proc, current);
	process_exit(proc, 0);
}

ZTEST_SUITE(process_management, NULL, NULL, NULL, NULL, NULL);
