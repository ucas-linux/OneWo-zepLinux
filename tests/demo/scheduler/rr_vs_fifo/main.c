#include <errno.h>
#include <lmz_sched.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

static void *fifo_worker(void *arg)
{
    ARG_UNUSED(arg);
    for (int i = 0; i < 8; ++i) {
        printf("[FIFO] iteration=%d\n", i);
        usleep(20000);
    }
    return NULL;
}

static void *rr_worker(void *arg)
{
    ARG_UNUSED(arg);
    for (int i = 0; i < 8; ++i) {
        printf("[RR  ] iteration=%d\n", i);
        usleep(20000);
    }
    return NULL;
}

int main(void)
{
    pthread_t fifo_thread;
    pthread_t rr_thread;
    struct sched_param param = { .sched_priority = 5 };

    printf("=== RR vs FIFO demo ===\n");
    printf("This demo compares two thread bodies with different LMZ policies.\n");

    if (pthread_create(&fifo_thread, NULL, fifo_worker, NULL) != 0) {
        printf("failed to create FIFO thread\n");
        return 1;
    }

    if (pthread_create(&rr_thread, NULL, rr_worker, NULL) != 0) {
        printf("failed to create RR thread\n");
        return 1;
    }

    int ret_fifo = lmz_sched_setscheduler(0, LMZ_SCHED_FIFO, &param);
    int ret_rr = lmz_sched_setscheduler(0, LMZ_SCHED_RR, &param);

    printf("lmz_sched_setscheduler(FIFO/current) => %d\n", ret_fifo);
    printf("lmz_sched_setscheduler(RR/current)   => %d\n", ret_rr);
    printf("Note: current LMZ API only resolves pid=0 to current thread.\n");
    printf("Use this demo to confirm build path and inspect behavior/logging.\n");

    pthread_join(fifo_thread, NULL);
    pthread_join(rr_thread, NULL);

    return 0;
}
