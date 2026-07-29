/* fork_demo.c — ANL module: fork two children via new_task
 *
 * Compile with:
 *   arm-zephyr-eabi-gcc -c -O1 -mthumb -mcpu=cortex-m3 \
 *       -fno-builtin -ffreestanding -mlong-calls -o fork_demo.o fork_demo.c
 *   python3 tools/anl_link.py fork_demo.o fork_demo.anl --entry main
 */
#include <stdint.h>

extern void printk(const char *fmt);
extern void k_msleep(int ms);
extern int  new_task(const char *name, void *(*fn)(void *), void *arg);

static void *child_fn(void *arg)
{
    int id = (int)(uintptr_t)arg;
    if (id == 1) {
        printk("[child] id=1\n");
        k_msleep(300);
        printk("[child1] done\n");
    } else {
        printk("[child] id=2\n");
        k_msleep(300);
        printk("[child2] done\n");
    }
    return arg;
}

void main(void)
{
    printk("[anl] main start\n");
    new_task("c1", child_fn, (void *)1);
    new_task("c2", child_fn, (void *)2);
    printk("[anl] spawned\n");
}
