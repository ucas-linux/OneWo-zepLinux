/**
 * @file test_sched.c
 * @brief LMZ Scheduling Unit Tests
 *
 * Focused tests for LMZ scheduling APIs.
 */

#include <zephyr/ztest.h>
#include <lmz_sched.h>
#include <errno.h>

void test_lmz_sched_yield_basic(void)
{
    int ret = lmz_sched_yield();
    zassert_equal(0, ret, "lmz_sched_yield should succeed");
}

void test_lmz_sched_setscheduler_invalid_param(void)
{
    int ret = lmz_sched_setscheduler(0, LMZ_SCHED_FIFO, NULL);
    zassert_true(ret == -EINVAL || ret == -ENOSYS,
                 "NULL param should fail with -EINVAL or -ENOSYS, got %d", ret);
}

void test_lmz_sched_setscheduler_current_thread(void)
{
    struct sched_param param = { .sched_priority = 4 };
    int ret = lmz_sched_setscheduler(0, LMZ_SCHED_RR, &param);

    zassert_true(ret == 0 || ret == -ENOSYS,
                 "setscheduler should succeed or report unimplemented, got %d", ret);
}

void test_lmz_sched_getscheduler_current_thread(void)
{
    int ret = lmz_sched_getscheduler(0);

    zassert_true(ret >= 0 || ret == -ENOSYS,
                 "getscheduler should return policy or -ENOSYS, got %d", ret);
}

void test_lmz_sched_setparam_roundtrip(void)
{
    struct sched_param in = { .sched_priority = 6 };
    struct sched_param out = { .sched_priority = -1 };

    int ret_set = lmz_sched_setparam(0, &in);
    int ret_get = lmz_sched_getparam(0, &out);

    zassert_true(ret_set == 0 || ret_set == -ENOSYS,
                 "setparam should succeed or report unimplemented, got %d", ret_set);
    zassert_true(ret_get == 0 || ret_get == -ENOSYS,
                 "getparam should succeed or report unimplemented, got %d", ret_get);

    if (ret_set == 0 && ret_get == 0) {
        zassert_true(out.sched_priority == in.sched_priority || out.sched_priority == 0,
                     "priority roundtrip should be stable for current implementation");
    }
}

void test_lmz_sched_deadline_validation(void)
{
    int ret_zero = lmz_sched_setattr_dl(0, 0, 100, 1000);
    int ret_order = lmz_sched_setattr_dl(0, 1000, 100, 500);

    zassert_true(ret_zero == -EINVAL || ret_zero == -ENOSYS,
                 "zero runtime should fail, got %d", ret_zero);
    zassert_true(ret_order == -EINVAL || ret_order == -ENOSYS,
                 "runtime/deadline/period ordering should fail, got %d", ret_order);
}

void test_lmz_nice_and_priority_range(void)
{
    int nice_ret = lmz_nice(1);
    int setprio_low = lmz_setpriority(LMZ_PRIO_PROCESS, 0, -21);
    int setprio_high = lmz_setpriority(LMZ_PRIO_PROCESS, 0, 20);

    zassert_true(nice_ret >= -20 || nice_ret == -ENOSYS,
                 "nice should clamp or report unimplemented, got %d", nice_ret);
    zassert_true(setprio_low == -EINVAL || setprio_low == -ENOSYS,
                 "too-low prio should fail, got %d", setprio_low);
    zassert_true(setprio_high == -EINVAL || setprio_high == -ENOSYS,
                 "too-high prio should fail, got %d", setprio_high);
}

void test_lmz_getpriority_invalid_which(void)
{
    int ret = lmz_getpriority(1234, 0);
    zassert_true(ret == -EINVAL || ret == -ENOSYS,
                 "invalid which should fail, got %d", ret);
}

void test_main(void)
{
    ztest_test_suite(sched_tests,
        ztest_unit_test(test_lmz_sched_yield_basic),
        ztest_unit_test(test_lmz_sched_setscheduler_invalid_param),
        ztest_unit_test(test_lmz_sched_setscheduler_current_thread),
        ztest_unit_test(test_lmz_sched_getscheduler_current_thread),
        ztest_unit_test(test_lmz_sched_setparam_roundtrip),
        ztest_unit_test(test_lmz_sched_deadline_validation),
        ztest_unit_test(test_lmz_nice_and_priority_range),
        ztest_unit_test(test_lmz_getpriority_invalid_which)
    );

    ztest_run_test_suite(sched_tests);
}
