/*
 * POSIX线程示例 - 生产者消费者模型
 *
 * 演示：
 * - pthread_create创建线程
 * - pthread_mutex互斥锁
 * - pthread_cond条件变量
 * - pthread_join等待线程
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUFFER_SIZE 10
#define NUM_ITEMS 20

// 共享缓冲区
int buffer[BUFFER_SIZE];
int count = 0;
int in = 0;
int out = 0;

// 同步原语
pthread_mutex_t mutex;
pthread_cond_t not_full;
pthread_cond_t not_empty;

// 生产者线程
void *producer(void *arg)
{
    int id = *(int *)arg;

    for (int i = 0; i < NUM_ITEMS; i++) {
        int item = id * 100 + i;

        pthread_mutex_lock(&mutex);

        // 等待缓冲区不满
        while (count == BUFFER_SIZE) {
            printf("Producer %d: buffer full, waiting...\n", id);
            pthread_cond_wait(&not_full, &mutex);
        }

        // 生产数据
        buffer[in] = item;
        in = (in + 1) % BUFFER_SIZE;
        count++;

        printf("Producer %d: produced %d (count=%d)\n", id, item, count);

        // 通知消费者
        pthread_cond_signal(&not_empty);

        pthread_mutex_unlock(&mutex);

        usleep(100000);  // 100ms
    }

    printf("Producer %d: finished\n", id);
    return NULL;
}

// 消费者线程
void *consumer(void *arg)
{
    int id = *(int *)arg;

    for (int i = 0; i < NUM_ITEMS; i++) {
        pthread_mutex_lock(&mutex);

        // 等待缓冲区不空
        while (count == 0) {
            printf("Consumer %d: buffer empty, waiting...\n", id);
            pthread_cond_wait(&not_empty, &mutex);
        }

        // 消费数据
        int item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        count--;

        printf("Consumer %d: consumed %d (count=%d)\n", id, item, count);

        // 通知生产者
        pthread_cond_signal(&not_full);

        pthread_mutex_unlock(&mutex);

        usleep(150000);  // 150ms
    }

    printf("Consumer %d: finished\n", id);
    return NULL;
}

int main(void)
{
    printf("POSIX Thread Example: Producer-Consumer\n");
    printf("========================================\n\n");

    // 初始化同步原语
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&not_full, NULL);
    pthread_cond_init(&not_empty, NULL);

    // 创建线程
    pthread_t prod1, prod2, cons1, cons2;
    int id1 = 1, id2 = 2, id3 = 3, id4 = 4;

    pthread_create(&prod1, NULL, producer, &id1);
    pthread_create(&prod2, NULL, producer, &id2);
    pthread_create(&cons1, NULL, consumer, &id3);
    pthread_create(&cons2, NULL, consumer, &id4);

    // 等待所有线程完成
    pthread_join(prod1, NULL);
    pthread_join(prod2, NULL);
    pthread_join(cons1, NULL);
    pthread_join(cons2, NULL);

    // 清理
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&not_full);
    pthread_cond_destroy(&not_empty);

    printf("\nAll threads finished successfully!\n");

    return 0;
}
