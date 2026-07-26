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
 * @brief 测试进程创建和PID分配
 */
ZTEST(process_as32x601, test_process_create)
{
	struct z_process *proc1, *proc2;

	printk("Testing process creation...\n");

	/* 创建第一个进程 */
	proc1 = process_create(NULL);
	zassert_not_null(proc1, "Failed to create process 1");
	zassert_true(proc1->pid > 0, "Invalid PID: %d", proc1->pid);
	printk("Process 1 created with PID: %d\n", proc1->pid);

	/* 创建第二个进程 */
	proc2 = process_create(NULL);
	zassert_not_null(proc2, "Failed to create process 2");
	zassert_true(proc2->pid > 0, "Invalid PID: %d", proc2->pid);
	printk("Process 2 created with PID: %d\n", proc2->pid);

	/* PID应该不同 */
	zassert_not_equal(proc1->pid, proc2->pid, "PIDs should be unique");
	printk("PIDs are unique: %d != %d\n", proc1->pid, proc2->pid);

	/* 清理 */
	process_exit(proc1, 0);
	process_exit(proc2, 0);

	printk("Process creation test PASSED\n");
}

/**
 * @brief 测试父子进程关系
 */
ZTEST(process_as32x601, test_parent_child)
{
	struct z_process *parent, *child;

	printk("Testing parent-child relationship...\n");

	/* 创建父进程 */
	parent = process_create(NULL);
	zassert_not_null(parent, "Failed to create parent process");
	printk("Parent process created with PID: %d\n", parent->pid);

	/* 创建子进程 */
	child = process_create(parent);
	zassert_not_null(child, "Failed to create child process");
	printk("Child process created with PID: %d\n", child->pid);

	/* 验证父子关系 */
	zassert_equal(child->parent, parent, "Child's parent pointer incorrect");
	printk("Parent-child relationship verified\n");

	/* 清理 */
	process_exit(child, 0);
	process_exit(parent, 0);

	printk("Parent-child test PASSED\n");
}

/**
 * @brief 测试文件描述符表
 */
ZTEST(process_as32x601, test_fd_table)
{
	struct z_process *proc;
	void *dummy_fd1 = (void *)0x1000;
	void *dummy_fd2 = (void *)0x2000;
	int fd1, fd2;
	void *retrieved;

	printk("Testing file descriptor table...\n");
	printk("Origin Zephyr\n");

	/* 创建进程 */
	proc = process_create(NULL);
	zassert_not_null(proc, "Failed to create process");

	/* 添加第一个FD */
	fd1 = process_idesc_table_add(proc, dummy_fd1);
	zassert_true(fd1 >= 0, "Failed to add fd1: %d", fd1);
	printk("FD1 allocated: %d\n", fd1);

	/* 添加第二个FD */
	fd2 = process_idesc_table_add(proc, dummy_fd2);
	zassert_true(fd2 >= 0, "Failed to add fd2: %d", fd2);
	printk("FD2 allocated: %d\n", fd2);

	/* 验证检索 */
	retrieved = process_idesc_table_get(proc, fd1);
	zassert_equal(retrieved, dummy_fd1, "Retrieved wrong fd1");

	retrieved = process_idesc_table_get(proc, fd2);
	zassert_equal(retrieved, dummy_fd2, "Retrieved wrong fd2");

	/* 移除fd1 */
	int ret = process_idesc_table_remove(proc, fd1);
	zassert_equal(ret, 0, "Failed to remove fd1");
	printk("FD1 removed successfully\n");

	/* fd1应该无效 */
	retrieved = process_idesc_table_get(proc, fd1);
	zassert_is_null(retrieved, "fd1 should be invalid after removal");

	/* 清理 */
	process_exit(proc, 0);

	printk("FD table test PASSED\n");
}

/**
 * @brief 测试环境变量
 */
ZTEST(process_as32x601, test_env_vars)
{
	struct z_process *proc;
	const char *value;
	int ret;

	printk("Testing environment variables...\n");

	/* 创建进程 */
	proc = process_create(NULL);
	zassert_not_null(proc, "Failed to create process");

	/* 设置环境变量 */
	ret = process_setenv(proc, "TEST_VAR", "test_value");
	zassert_equal(ret, 0, "Failed to set env variable");
	printk("Set TEST_VAR=test_value\n");

	/* 获取环境变量 */
	value = process_getenv(proc, "TEST_VAR");
	zassert_not_null(value, "Failed to get env variable");
	zassert_equal(strcmp(value, "test_value"), 0, "Wrong env value");
	printk("Got TEST_VAR=%s\n", value);

	/* 更新环境变量 */
	ret = process_setenv(proc, "TEST_VAR", "new_value");
	zassert_equal(ret, 0, "Failed to update env variable");

	value = process_getenv(proc, "TEST_VAR");
	zassert_equal(strcmp(value, "new_value"), 0, "Wrong updated value");
	printk("Updated TEST_VAR=%s\n", value);

	/* 清理 */
	process_exit(proc, 0);

	printk("Environment variables test PASSED\n");
}

/**
 * @brief 测试进程fork
 */
ZTEST(process_as32x601, test_fork)
{
	struct z_process *parent, *child;
	const char *value;
	int ret;

	printk("Testing process fork...\n");

	/* 创建父进程 */
	parent = process_create(NULL);
	zassert_not_null(parent, "Failed to create parent process");

	/* 在父进程设置环境变量 */
	ret = process_setenv(parent, "PARENT_VAR", "parent_value");
	zassert_equal(ret, 0, "Failed to set parent env");
	printk("Parent: Set PARENT_VAR=parent_value\n");

	/* Fork进程 */
	child = process_fork(parent);
	zassert_not_null(child, "Failed to fork process");
	printk("Child forked with PID: %d\n", child->pid);

	/* 验证环境变量被复制 */
	value = process_getenv(child, "PARENT_VAR");
	zassert_not_null(value, "Child should inherit parent's env");
	zassert_equal(strcmp(value, "parent_value"), 0, "Wrong inherited value");
	printk("Child: Inherited PARENT_VAR=%s\n", value);

	/* 修改子进程环境不应影响父进程 */
	ret = process_setenv(child, "PARENT_VAR", "child_value");
	zassert_equal(ret, 0, "Failed to update child env");

	value = process_getenv(parent, "PARENT_VAR");
	zassert_equal(strcmp(value, "parent_value"), 0,
	              "Parent env should not be affected");
	printk("Parent env unchanged: PARENT_VAR=%s\n", value);

	/* 清理 */
	process_exit(child, 0);
	process_exit(parent, 0);

	printk("Fork test PASSED\n");
}

/**
 * @brief 测试getpid()
 */
ZTEST(process_as32x601, test_getpid_basic)
{
	struct z_process *proc;
	pid_t init_pid;

	printk("Testing getpid()...\n");

	/* 创建一个进程 */
	proc = process_create(NULL);
	zassert_not_null(proc, "Failed to create process");
	printk("Process created with PID: %d\n", proc->pid);

	/* getpid应该返回init进程的PID（因为当前线程未注册到新进程） */
	init_pid = getpid();
	printk("Current getpid() returns: %d\n", init_pid);
	zassert_true(init_pid > 0, "getpid should return valid PID");

	/* 清理 */
	process_exit(proc, 0);

	printk("getpid test PASSED\n");
}

/* 测试套件设置 */
static void *process_setup(void)
{
	printk("\n=== AS32X601 Process Management Test Suite ===\n");
	printk("Starting tests...\n\n");
	return NULL;
}

static void process_teardown(void *fixture)
{
	printk("\n=== All Tests Completed ===\n");
}

ZTEST_SUITE(process_as32x601, NULL, process_setup, NULL, NULL, process_teardown);
