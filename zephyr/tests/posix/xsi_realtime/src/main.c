/*
 * Copyright (c) 2025 Marvin Ouma <pancakesdeath@protonmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <pthread.h>

#include <zephyr/kernel.h>
#include <zephyr/sys/timeutil.h>
#include <zephyr/ztest.h>

struct sched_thread_info {
	int policy;
	int priority;
	int getsched_ret;
	int interval_ret;
	struct timespec interval;
};

static void *sched_policy_info_thread(void *arg)
{
	struct sched_thread_info *info = arg;
	struct sched_param param = { 0 };

	info->getsched_ret = pthread_getschedparam(pthread_self(), &info->policy, &param);
	if (info->getsched_ret == 0) {
		info->priority = param.sched_priority;
	}

	info->interval_ret = sched_rr_get_interval(0, &info->interval);
	return NULL;
}

static void run_sched_policy_info_test(int policy, struct sched_thread_info *info)
{
	pthread_t thread;
	pthread_attr_t attr;
	struct sched_param thread_param;

	zassert_ok(pthread_attr_init(&attr));
	zassert_ok(pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED));
	zassert_ok(pthread_attr_setschedpolicy(&attr, policy));

	thread_param.sched_priority = MIN(1, sched_get_priority_max(policy));
	zassert_ok(pthread_attr_setschedparam(&attr, &thread_param));

	*info = (struct sched_thread_info){ 0 };
	zassert_ok(pthread_create(&thread, &attr, sched_policy_info_thread, info));
	zassert_ok(pthread_join(thread, NULL));

	zassert_ok(pthread_attr_destroy(&attr));
}

ZTEST(xsi_realtime, test_sched_getparam)
{
	int policy;
	struct sched_param param;
	int rc = sched_getparam(0, &param);

	zassert_ok(rc);
	policy = sched_getscheduler(0);
	zassert_true(policy == SCHED_FIFO || policy == SCHED_RR || policy == SCHED_OTHER);
	zassert_true(param.sched_priority >= sched_get_priority_min(policy));
	zassert_true(param.sched_priority <= sched_get_priority_max(policy));
}

ZTEST(xsi_realtime, test_sched_getscheduler)
{
	int rc = sched_getscheduler(0);

	zassert_true(rc == SCHED_FIFO || rc == SCHED_RR || rc == SCHED_OTHER);
}

ZTEST(xsi_realtime, test_sched_setparam)
{
	int policy;
	int max_prio;
	int old_policy;
	struct sched_param old_param;
	struct sched_param param = {
		.sched_priority = 0,
	};
	struct sched_param check;

	old_policy = sched_getscheduler(0);
	zassert_true(old_policy >= 0);
	zassert_ok(sched_getparam(0, &old_param));

	policy = old_policy;
	max_prio = sched_get_priority_max(policy);
	param.sched_priority = MIN(max_prio, 1);

	zassert_ok(sched_setparam(0, &param));
	zassert_ok(sched_getparam(0, &check));
	zassert_equal(check.sched_priority, param.sched_priority);

	if (old_policy == SCHED_OTHER) {
		old_policy = SCHED_RR;
	}
	(void)sched_setscheduler(0, old_policy, &old_param);
}

ZTEST(xsi_realtime, test_sched_setscheduler)
{
	int old_policy;
	int prev_policy;
	int new_policy;
	struct sched_param old_param;
	struct sched_param param = {
		.sched_priority = 1,
	};
	struct sched_param check;

	old_policy = sched_getscheduler(0);
	zassert_true(old_policy >= 0);
	zassert_ok(sched_getparam(0, &old_param));

	new_policy = (old_policy == SCHED_FIFO) ? SCHED_RR : SCHED_FIFO;
	param.sched_priority = MIN(sched_get_priority_max(new_policy), 1);

	prev_policy = sched_setscheduler(0, new_policy, &param);
	zassert_equal(prev_policy, old_policy);
	zassert_equal(sched_getscheduler(0), new_policy);
	zassert_ok(sched_getparam(0, &check));
	zassert_equal(check.sched_priority, param.sched_priority);

	if (old_policy == SCHED_OTHER) {
		old_policy = SCHED_RR;
	}
	(void)sched_setscheduler(0, old_policy, &old_param);
}

ZTEST(xsi_realtime, test_sched_rr_get_interval)
{
	struct sched_param old_param;
	struct sched_param rr_param;
	struct timespec interval = {
		.tv_sec = 0,
		.tv_nsec = 0,
	};
	struct timespec expected = SYS_TICKS_TO_TIMESPEC(k_ms_to_ticks_ceil32(CONFIG_TIMESLICE_SIZE));
	int old_policy = sched_getscheduler(0);

	zassert_true(old_policy >= 0);
	zassert_ok(sched_getparam(0, &old_param));

	rr_param.sched_priority = MIN(sched_get_priority_max(SCHED_RR), 1);
	zassert_true(sched_setscheduler(0, SCHED_RR, &rr_param) >= 0);
	zassert_ok(sched_rr_get_interval(0, &interval));
	zassert_equal(interval.tv_sec, expected.tv_sec);
	zassert_equal(interval.tv_nsec, expected.tv_nsec);

	if (old_policy == SCHED_OTHER) {
		old_policy = SCHED_RR;
	}
	(void)sched_setscheduler(0, old_policy, &old_param);
}

ZTEST(xsi_realtime, test_sched_rr_thread_reports_rr_interval)
{
	struct sched_thread_info info;
	struct timespec expected = SYS_TICKS_TO_TIMESPEC(k_ms_to_ticks_ceil32(CONFIG_TIMESLICE_SIZE));

	run_sched_policy_info_test(SCHED_RR, &info);

	zassert_ok(info.getsched_ret);
	zassert_ok(info.interval_ret);
	zassert_equal(info.policy, SCHED_RR);
	zassert_equal(info.priority, MIN(1, sched_get_priority_max(SCHED_RR)));
	zassert_equal(info.interval.tv_sec, expected.tv_sec);
	zassert_equal(info.interval.tv_nsec, expected.tv_nsec);
}

ZTEST(xsi_realtime, test_sched_fifo_thread_reports_no_rr_interval)
{
	struct sched_thread_info info;

	run_sched_policy_info_test(SCHED_FIFO, &info);

	zassert_ok(info.getsched_ret);
	zassert_ok(info.interval_ret);
	zassert_equal(info.policy, SCHED_FIFO);
	zassert_equal(info.priority, MIN(1, sched_get_priority_max(SCHED_FIFO)));
	zassert_equal(info.interval.tv_sec, 0);
	zassert_equal(info.interval.tv_nsec, 0);
}

ZTEST_SUITE(xsi_realtime, NULL, NULL, NULL, NULL, NULL);
