/**
 * @file test_stress.c
 * @brief LMZ Stress Tests
 *
 * Stress tests for resource limits and concurrent access.
 *
 * Copyright (c) 2025 LMZ Project
 */

#include <zephyr/ztest.h>
#include <pthread.h>
#include <stdlib.h>

#define MAX_THREADS 16
#define MAX_ITERATIONS 10000
#define STRESS_DURATION_SECONDS 5

/* Shared data */
static pthread_mutex_t stress_mutex;
static int stress_counter = 0;
static bool stress_complete = false;

/**
 * @brief Stress thread function
 */
void *stress_thread(void *arg)
{
    (void)arg;

    while (!stress_complete) {
        pthread_mutex_lock(&stress_mutex);
        stress_counter++;
        pthread_mutex_unlock(&stress_mutex);
    }

    return NULL;
}

/**
 * @brief Test maximum concurrent threads
 */
void test_max_threads(void)
{
    pthread_t threads[MAX_THREADS];
    int created = 0;

    pthread_mutex_init(&stress_mutex, NULL);

    /* Try to create maximum threads */
    for (int i = 0; i < MAX_THREADS; i++) {
        if (pthread_create(&threads[i], NULL, stress_thread, NULL) == 0) {
            created++;
        } else {
            break;
        }
    }

    printf("Created %d threads\n", created);
    zassert_true(created > 0, "should create at least one thread");
    zassert_true(created <= MAX_THREADS, "should not exceed max");

    /* Wait for stress period */
    k_sleep(K_SECONDS(1));

    /* Stop threads */
    stress_complete = true;

    /* Join all threads */
    for (int i = 0; i < created; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Counter value: %d\n", stress_counter);

    pthread_mutex_destroy(&stress_mutex);

    stress_complete = false;
    stress_counter = 0;
}

/**
 * @brief Test memory allocation stress
 */
void test_memory_stress(void)
{
    void *ptrs[50];
    int allocated = 0;

    /* Allocate many blocks */
    for (int i = 0; i < 50; i++) {
        ptrs[i] = malloc(1024);
        if (ptrs[i] != NULL) {
            allocated++;
            *((int *)ptrs[i]) = i;
        }
    }

    printf("Allocated %d blocks\n", allocated);
    zassert_true(allocated > 0, "should allocate at least one block");

    /* Verify data integrity */
    for (int i = 0; i < allocated; i++) {
        zassert_equal(i, *((int *)ptrs[i]),
                     "data integrity should be preserved");
    }

    /* Free all blocks */
    for (int i = 0; i < allocated; i++) {
        free(ptrs[i]);
    }
}

/**
 * @brief Test mutex contention
 */
void test_mutex_contention(void)
{
    pthread_t threads[8];
    stress_counter = 0;
    stress_complete = false;

    pthread_mutex_init(&stress_mutex, NULL);

    /* Create contention threads */
    for (int i = 0; i < 8; i++) {
        pthread_create(&threads[i], NULL, stress_thread, NULL);
    }

    /* Let threads run */
    k_sleep(K_SECONDS(2));

    /* Stop threads */
    stress_complete = true;

    for (int i = 0; i < 8; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Contention counter: %d\n", stress_counter);
    zassert_true(stress_counter > 0, "threads should have run");

    pthread_mutex_destroy(&stress_mutex);
    stress_complete = false;
    stress_counter = 0;
}

/**
 * @brief Test memory fragmentation
 */
void test_memory_fragmentation(void)
{
    void *ptrs[20];

    /* Allocate many small blocks */
    for (int i = 0; i < 20; i++) {
        ptrs[i] = malloc(100);
        zassert_not_null(ptrs[i], "allocation %d should succeed", i);
    }

    /* Free every other one */
    for (int i = 0; i < 20; i += 2) {
        free(ptrs[i]);
        ptrs[i] = NULL;
    }

    /* Allocate larger blocks in gaps */
    for (int i = 0; i < 20; i += 2) {
        ptrs[i] = malloc(200);
        zassert_not_null(ptrs[i],
                     "realloc after free should succeed", i);
    }

    /* Clean up */
    for (int i = 0; i < 20; i++) {
        if (ptrs[i] != NULL) {
            free(ptrs[i]);
        }
    }
}

/**
 * @brief Test suite entry
 */
void test_main(void)
{
    ztest_test_suite(stress_tests,
        ztest_unit_test(test_max_threads),
        ztest_unit_test(test_memory_stress),
        ztest_unit_test(test_mutex_contention),
        ztest_unit_test(test_memory_fragmentation)
    );

    ztest_run_test_suite(stress_tests);
}
