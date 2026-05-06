/**
 * @file test_thread.c
 * @brief LMZ Thread Management Unit Tests
 *
 * Unit tests for pthread interfaces using Zephyr's ztest framework.
 *
 * Copyright (c) 2025 LMZ Project
 */

#include <zephyr/ztest.h>
#include <pthread.h>
#include <stdio.h>

/* Test data */
static int test_value = 0;
static bool thread_ran = false;

/**
 * @brief Simple test thread function
 */
void *dummy_thread(void *arg)
{
    (void)arg;
    thread_ran = true;
    return NULL;
}

/**
 * @brief Increment thread function for mutex tests
 */
void *increment_thread(void *arg)
{
    pthread_mutex_t *mutex = (pthread_mutex_t *)arg;

    for (int i = 0; i < 100; i++) {
        pthread_mutex_lock(mutex);
        test_value++;
        pthread_mutex_unlock(mutex);
    }

    return NULL;
}

/**
 * @brief Test basic thread creation
 */
void test_pthread_create_basic(void)
{
    pthread_t thread;

    int rc = pthread_create(&thread, NULL, dummy_thread, NULL);
    zassert_equal(0, rc, "pthread_create should succeed");

    pthread_join(thread, NULL);
    zassert_true(thread_ran, "thread should have executed");
}

/**
 * @brief Test thread stack allocation
 */
void test_pthread_stack_allocation(void)
{
    pthread_t thread;
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr, 2048);

    int rc = pthread_create(&thread, &attr, dummy_thread, NULL);
    zassert_equal(0, rc, "create with custom stack should succeed");

    pthread_join(thread, NULL);
    zassert_true(thread_ran, "thread should have executed");

    pthread_attr_destroy(&attr);
}

/**
 * @brief Test basic mutex lock/unlock
 */
void test_pthread_mutex_basic(void)
{
    pthread_mutex_t mutex;
    int rc = pthread_mutex_init(&mutex, NULL);
    zassert_equal(0, rc, "mutex init should succeed");

    pthread_mutex_lock(&mutex);
    test_value = 42;
    pthread_mutex_unlock(&mutex);

    zassert_equal(42, test_value, "mutex should protect critical section");

    pthread_mutex_destroy(&mutex);
}

/**
 * @brief Test concurrent mutex access
 */
void test_pthread_mutex_concurrent(void)
{
    pthread_mutex_t mutex;
    pthread_t thread1, thread2;

    test_value = 0;
    pthread_mutex_init(&mutex, NULL);

    pthread_create(&thread1, NULL, increment_thread, &mutex);
    pthread_create(&thread2, NULL, increment_thread, &mutex);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    zassert_equal(200, test_value, "counter should be 200");

    pthread_mutex_destroy(&mutex);
}

/**
 * @brief Test malloc basic functionality
 */
void test_malloc_basic(void)
{
    void *ptr = malloc(100);
    zassert_not_null(ptr, "malloc should succeed");

    *((int *)ptr) = 42;
    zassert_equal(42, *((int *)ptr), "allocated memory should be writable");

    free(ptr);
}

/**
 * @brief Test free NULL pointer
 */
void test_free_null(void)
{
    free(NULL);  /* Should not crash */
    zassert_true(true, "free NULL should be safe");
}

/**
 * @brief Test memory reallocation
 */
void test_realloc_basic(void)
{
    void *ptr = malloc(100);
    zassert_not_null(ptr, "malloc should succeed");

    *((int *)ptr) = 42;

    void *new_ptr = realloc(ptr, 200);
    zassert_not_null(new_ptr, "realloc should succeed");

    zassert_equal(42, *((int *)new_ptr), "data should be preserved");

    free(new_ptr);
}

/**
 * @brief Test suite entry
 */
void test_main(void)
{
    ztest_test_suite(thread_tests,
        ztest_unit_test(test_pthread_create_basic),
        ztest_unit_test(test_pthread_stack_allocation),
        ztest_unit_test(test_pthread_mutex_basic),
        ztest_unit_test(test_pthread_mutex_concurrent),
        ztest_unit_test(test_malloc_basic),
        ztest_unit_test(test_free_null),
        ztest_unit_test(test_realloc_basic)
    );

    ztest_run_test_suite(thread_tests);
}
