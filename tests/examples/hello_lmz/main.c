/**
 * @file main.c
 * @brief LMZ Hello World Example
 *
 * Simple example demonstrating basic LMZ usage.
 *
 * Copyright (c) 2025 LMZ Project
 */

#include <lmz.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

/**
 * @brief Example thread function
 */
void *hello_thread(void *arg)
{
    const char *name = (const char *)arg;

    for (int i = 0; i < 3; i++) {
        printf("Hello from %s thread! (%d/3)\n", name, i + 1);
        sleep(1);
    }

    printf("%s thread done\n", name);
    return NULL;
}

/**
 * @brief Main function
 */
int main(void)
{
    /* Initialize LMZ subsystem */
    int ret = lmz_init();
    if (ret != 0) {
        printf("Failed to initialize LMZ: %d\n", ret);
        return 1;
    }

    printf("\n=== LMZ Hello World ===\n");
    printf("Platform: ARM Cortex-M3\n");
    printf("RTOS: Zephyr\n");
    printf("Interfaces: LMZ Linux compatibility layer\n");
    printf("\n");

    /* Example 1: Thread creation */
    printf("Example 1: Creating threads...\n");

    pthread_t thread1, thread2;

    ret = pthread_create(&thread1, NULL, hello_thread, "Thread-1");
    if (ret != 0) {
        printf("Failed to create thread 1: %d\n", ret);
        lmz_cleanup();
        return 1;
    }

    ret = pthread_create(&thread2, NULL, hello_thread, "Thread-2");
    if (ret != 0) {
        printf("Failed to create thread 2: %d\n", ret);
        lmz_cleanup();
        return 1;
    }

    /* Wait for threads to complete */
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("\n");

    /* Example 2: Mutex usage */
    printf("Example 2: Using mutex...\n");

    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);

    int counter = 0;
    const int TARGET = 1000;

    for (int i = 0; i < TARGET; i++) {
        pthread_mutex_lock(&mutex);
        counter++;
        pthread_mutex_unlock(&mutex);
    }

    printf("Counter value: %d (expected: %d)\n", counter, TARGET);
    printf("Mutex usage successful\n");

    pthread_mutex_destroy(&mutex);

    printf("\n");

    /* Example 3: Memory allocation */
    printf("Example 3: Memory allocation...\n");

    void *ptr1 = malloc(1024);
    void *ptr2 = malloc(2048);
    void *ptr3 = malloc(512);

    printf("Allocated 3 blocks: 1024, 2048, 512 bytes\n");

    if (ptr1 && ptr2 && ptr3) {
        printf("All allocations successful\n");

        free(ptr1);
        free(ptr2);
        free(ptr3);

        printf("All blocks freed\n");
    } else {
        printf("Allocation failed\n");
    }

    printf("\n");

    /* Example 4: Sleep/timer */
    printf("Example 4: Sleep and timers...\n");

    printf("Sleeping for 2 seconds...\n");
    sleep(2);

    printf("Using usleep...\n");
    usleep(500000);  // 500ms
    printf("usleep complete\n");

    printf("\n");

    /* Final summary */
    printf("=== LMZ Hello World Complete ===\n");
    printf("All examples ran successfully!\n");
    printf("\n");

    /* Cleanup and exit */
    lmz_cleanup();

    return 0;
}
