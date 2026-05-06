#include <lmz_sched.h>
#include <stdio.h>

int main(void)
{
    struct sched_param param = { .sched_priority = 3 };

    printf("=== RR interval demo ===\n");
    printf("Configured Zephyr timeslice size: 10 ticks (from prj.conf).\n");

    int ret = lmz_sched_setscheduler(0, LMZ_SCHED_RR, &param);
    int policy = lmz_sched_getscheduler(0);

    printf("setscheduler(RR) ret : %d\n", ret);
    printf("current policy       : %d\n", policy);
    printf("Current LMZ layer does not yet expose sched_rr_get_interval().\n");
    printf("Use this demo to verify RR policy enablement before adding that API.\n");

    return 0;
}
