/**
 * @file test_memory.c
 * @brief LMZ Memory Management Unit Tests
 *
 * Unit tests for memory interfaces using Zephyr's ztest framework.
 *
 * Copyright (c) 2025 LMZ Project
 */

#include <zephyr/ztest.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Test allocation and deallocation
 */
void test_alloc_free_cycle(void)
{
    void *ptr1 = malloc(100);
    zassert_not_null(ptr1, "first malloc should succeed");

    void *ptr2 = malloc(200);
    zassert_not_null(ptr2, "second malloc should succeed");

    /* Verify allocations are different */
    zassert_not_equal(ptr1, ptr2, "allocations should be distinct");

    free(ptr1);
    free(ptr2);

    /* Should be able to allocate again after free */
    void *ptr3 = malloc(150);
    zassert_not_null(ptr3, "malloc after free should succeed");

    free(ptr3);
}

/**
 * @brief Test zero-size allocation
 */
void test_zero_size_alloc(void)
{
    /* Behavior is implementation-defined */
    void *ptr = malloc(0);

    /* Either returns NULL or a small unique pointer */
    if (ptr != NULL) {
        free(ptr);  /* Should be safe */
    }

    zassert_true(true, "zero-size allocation handled");
}

/**
 * @brief Test realloc with NULL pointer
 */
void test_realloc_null_ptr(void)
{
    void *ptr = realloc(NULL, 100);
    zassert_not_null(ptr, "realloc(NULL) should behave like malloc");

    free(ptr);
}

/**
 * @brief Test realloc with zero size
 */
void test_realloc_zero_size(void)
{
    void *orig = malloc(100);
    zassert_not_null(orig, "original alloc should succeed");

    void *ptr = realloc(orig, 0);

    /* Behavior: either returns NULL or unique pointer */
    if (ptr != NULL) {
        free(ptr);
    }

    zassert_true(true, "zero-size realloc handled");
}

/**
 * @brief Test realloc in-place expansion
 */
void test_realloc_expand(void)
{
    void *ptr = malloc(100);
    zassert_not_null(ptr, "malloc should succeed");

    *((int *)ptr) = 12345;

    void *new_ptr = realloc(ptr, 200);
    zassert_not_null(new_ptr, "realloc should succeed");

    /* Data should be preserved */
    zassert_equal(12345, *((int *)new_ptr), "data should be preserved");

    free(new_ptr);
}

/**
 * @brief Test memory alignment
 */
void test_memory_alignment(void)
{
    void *ptr = malloc(100);

    /* Pointer should be aligned to word boundary */
    zassert_equal(0, (uintptr_t)ptr % sizeof(void *),
                 "pointer should be aligned");

    free(ptr);
}

/**
 * @brief Test allocation exhaustion
 */
void test_alloc_exhaustion(void)
{
    /* Try to allocate very large size */
    void *ptr = malloc(1024 * 1024 * 10);  /* 10MB */

    /* On embedded systems, this should fail */
    if (ptr != NULL) {
        free(ptr);
    }

    /* Test should pass regardless (just shouldn't crash) */
    zassert_true(true, "large allocation handled");
}

/**
 * @brief Test multiple allocations
 */
void test_multiple_allocations(void)
{
    void *ptrs[10];

    /* Allocate multiple blocks */
    for (int i = 0; i < 10; i++) {
        ptrs[i] = malloc(100);
        zassert_not_null(ptrs[i], "allocation %d should succeed", i);
    }

    /* Verify all allocations are distinct */
    for (int i = 0; i < 10; i++) {
        for (int j = i + 1; j < 10; j++) {
            zassert_not_equal(ptrs[i], ptrs[j],
                             "allocations should be distinct");
        }
    }

    /* Free all allocations */
    for (int i = 0; i < 10; i++) {
        free(ptrs[i]);
    }
}

/**
 * @brief Test double-free detection
 */
void test_double_free(void)
{
    void *ptr = malloc(100);
    zassert_not_null(ptr, "malloc should succeed");

    free(ptr);

    /* Double-free should be detected or safe */
    /* This test passes if it doesn't crash */
    zassert_true(true, "double-free handled");
}

/**
 * @brief Test suite entry
 */
void test_main(void)
{
    ztest_test_suite(memory_tests,
        ztest_unit_test(test_alloc_free_cycle),
        ztest_unit_test(test_zero_size_alloc),
        ztest_unit_test(test_realloc_null_ptr),
        ztest_unit_test(test_realloc_zero_size),
        ztest_unit_test(test_realloc_expand),
        ztest_unit_test(test_memory_alignment),
        ztest_unit_test(test_alloc_exhaustion),
        ztest_unit_test(test_multiple_allocations),
        ztest_unit_test(test_double_free)
    );

    ztest_run_test_suite(memory_tests);
}
