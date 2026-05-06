/*
 * Copyright (c) 2018-2023 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "pthread_sched.h"

#include <zephyr/kernel.h>
#include <zephyr/posix/sched.h>
#include <zephyr/posix/unistd.h>
#include <zephyr/sys/timeutil.h>
#include <kthread.h>
#include <ksched.h>

static struct k_thread *sched_pid_to_thread(pid_t pid)
{
	if (pid == 0 || pid == getpid()) {
		return k_current_get();
	}

	errno = ESRCH;
	return NULL;
}

/**
 * @brief Get minimum priority value for a given policy
 *
 * See IEEE 1003.1
 */
int sched_get_priority_min(int policy)
{
	return posix_sched_priority_min(policy);
}

/**
 * @brief Get maximum priority value for a given policy
 *
 * See IEEE 1003.1
 */
int sched_get_priority_max(int policy)
{
	return posix_sched_priority_max(policy);
}

/**
 * @brief Get scheduling parameters
 *
 * See IEEE 1003.1
 */
int sched_getparam(pid_t pid, struct sched_param *param)
{
	struct k_thread *thread;

	if (param == NULL) {
		errno = EINVAL;
		return -1;
	}

	thread = sched_pid_to_thread(pid);
	if (thread == NULL) {
		return -1;
	}

	K_SPINLOCK(&_sched_spinlock) {
		int policy = zephyr_policy_to_posix_policy(
			z_thread_sched_policy_get(thread));

		param->sched_priority =
			zephyr_prio_to_posix_priority(thread->base.prio, policy);
	}

	return 0;
}

/**
 * @brief Get scheduling policy
 *
 * See IEEE 1003.1
 */
int sched_getscheduler(pid_t pid)
{
	struct k_thread *thread = sched_pid_to_thread(pid);

	if (thread == NULL) {
		return -1;
	}

	K_SPINLOCK(&_sched_spinlock) {
		pid = zephyr_policy_to_posix_policy(z_thread_sched_policy_get(thread));
	}

	return pid;
}

/**
 * @brief Set scheduling parameters
 *
 * See IEEE 1003.1
 */
int sched_setparam(pid_t pid, const struct sched_param *param)
{
	struct k_thread *thread;
	int policy;
	int prio;

	if (param == NULL) {
		errno = EINVAL;
		return -1;
	}

	thread = sched_pid_to_thread(pid);
	if (thread == NULL) {
		return -1;
	}

	K_SPINLOCK(&_sched_spinlock) {
		policy = zephyr_policy_to_posix_policy(z_thread_sched_policy_get(thread));
	}

	if (!is_posix_policy_prio_valid(param->sched_priority, policy)) {
		errno = EINVAL;
		return -1;
	}

	prio = posix_to_zephyr_priority(param->sched_priority, policy);
	z_thread_priority_and_policy_set(thread, prio,
					 posix_policy_to_zephyr_policy(policy));

	return 0;
}

/**
 * @brief Set scheduling policy
 *
 * See IEEE 1003.1
 */
int sched_setscheduler(pid_t pid, int policy, const struct sched_param *param)
{
	struct k_thread *thread;
	int old_policy;
	int prio;

	if (param == NULL || !valid_posix_policy(policy) ||
	    !is_posix_policy_prio_valid(param->sched_priority, policy)) {
		errno = EINVAL;
		return -1;
	}

	thread = sched_pid_to_thread(pid);
	if (thread == NULL) {
		return -1;
	}

	prio = posix_to_zephyr_priority(param->sched_priority, policy);
	old_policy = sched_getscheduler(pid);
	z_thread_priority_and_policy_set(thread, prio,
					 posix_policy_to_zephyr_policy(policy));

	return old_policy;
}

int sched_rr_get_interval(pid_t pid, struct timespec *interval)
{
	struct k_thread *thread;
	int32_t slice_ticks = 0;

	if (interval == NULL) {
		errno = EINVAL;
		return -1;
	}

	thread = sched_pid_to_thread(pid);
	if (thread == NULL) {
		return -1;
	}

	K_SPINLOCK(&_sched_spinlock) {
		slice_ticks = z_get_thread_timeslice_ticks(thread);
	}

	*interval = SYS_TICKS_TO_TIMESPEC(slice_ticks);
	return 0;
}
