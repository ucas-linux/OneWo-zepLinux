#include <lmz_sched.h>
#include <stdio.h>

int main(void)
{
    struct sched_param param = { .sched_priority = 7 };
    struct sched_param out = {0};

    printf("=== Priority mapping demo ===\n");

    int ret_set = lmz_sched_setparam(0, &param);
    int ret_get = lmz_sched_getparam(0, &out);
    int ret_policy = lmz_sched_getscheduler(0);

    printf("setparam ret      : %d\n", ret_set);
    printf("getparam ret      : %d\n", ret_get);
    printf("getscheduler ret  : %d\n", ret_policy);
    printf("reported priority : %d\n", out.sched_priority);

    printf("Interpretation:\n");
    printf("- if CONFIG_SCHED_LINUX is disabled, the calls may return -ENOSYS.\n");
    printf("- if enabled, this demo helps verify the set/get round trip.\n");

    return 0;
}
