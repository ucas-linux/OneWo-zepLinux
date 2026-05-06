/*
 * Copyright (c) 2023 Meta
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_LIB_POSIX_POSIX_PTHREAD_SCHED_H_
#define ZEPHYR_LIB_POSIX_POSIX_PTHREAD_SCHED_H_

#include <errno.h>
#include <stdbool.h>

#include <zephyr/kernel.h>
#include <zephyr/posix/sched.h>

static inline bool valid_posix_policy(int policy)
{
	return policy == SCHED_FIFO || policy == SCHED_RR || policy == SCHED_OTHER;
}

static inline int posix_policy_to_zephyr_policy(int policy)
{
	return (policy == SCHED_OTHER) ? Z_THREAD_SCHED_OTHER : policy;
}

static inline int zephyr_policy_to_posix_policy(int policy)
{
	return (policy == Z_THREAD_SCHED_OTHER) ? SCHED_OTHER : policy;
}

static inline int posix_sched_priority_min(int policy)
{
	if (!valid_posix_policy(policy)) {
		errno = EINVAL;
		return -1;
	}

	return 0;
}

static inline int posix_sched_priority_max(int policy)
{
	if (IS_ENABLED(CONFIG_COOP_ENABLED) && policy == SCHED_FIFO) {
		return CONFIG_NUM_COOP_PRIORITIES - 1;
	} else if (IS_ENABLED(CONFIG_PREEMPT_ENABLED) &&
		   (policy == SCHED_RR || policy == SCHED_OTHER)) {
		return CONFIG_NUM_PREEMPT_PRIORITIES - 1;
	}

	errno = EINVAL;
	return -1;
}

static inline bool is_posix_policy_prio_valid(int priority, int policy)
{
	if (priority >= posix_sched_priority_min(policy) &&
	    priority <= posix_sched_priority_max(policy)) {
		return true;
	}

	return false;
}

int zephyr_to_posix_priority(int z_prio, int *policy);
int zephyr_prio_to_posix_priority(int z_prio, int policy);
int posix_to_zephyr_priority(int priority, int policy);

#endif
