#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/kernel/process.h>
#include <string.h>
#include <stdlib.h>
#include "anl_loader.h"
#include "shell_process.h"

static const struct device *uart_dev;

/* Exported symbols available to loaded ANL modules */
static void anl_printk_wrapper(const char *fmt)
{
    printk("%s", fmt);
}

const struct anl_export _anl_exports[] = {
    { "printk",          (uintptr_t)anl_printk_wrapper },
    { "k_msleep",        (uintptr_t)k_msleep },
    { "new_task",        (uintptr_t)new_task },
    { "waitpid",         (uintptr_t)waitpid },
    { "process_current", (uintptr_t)process_current },
};
const int _anl_exports_count = 5;

static uint8_t anl_buf[4096];

static int hex_nibble(char c)
{
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

static void uart_putc(char c)
{
    uart_poll_out(uart_dev, c);
}

static void uart_puts(const char *s)
{
    while (*s) {
        if (*s == '\n') uart_putc('\r');
        uart_putc(*s++);
    }
}

static int readline(char *buf, int maxlen)
{
    int i = 0;
    while (i < maxlen - 1) {
        unsigned char c;
        while (uart_poll_in(uart_dev, &c) != 0)
            k_sleep(K_MSEC(1));
        if (c == '\r' || c == '\n') {
            buf[i] = '\0';
            uart_puts("\r\n");
            return i;
        }
        if (c == '\b' || c == 127) {
            if (i > 0) { i--; uart_puts("\b \b"); }
            continue;
        }
        uart_putc(c);
        buf[i++] = c;
    }
    buf[i] = '\0';
    return i;
}

static void cmd_load(char *args)
{
    char *name = args;
    char *hex = strchr(args, ' ');
    if (!hex) { uart_puts("usage: load <name> <hexdata>\n"); return; }
    *hex++ = '\0';

    size_t hexlen = strlen(hex);
    if (hexlen & 1) { uart_puts("error: odd hex length\n"); return; }
    size_t binlen = hexlen / 2;
    if (binlen > sizeof(anl_buf)) { uart_puts("error: too large\n"); return; }

    for (size_t i = 0; i < binlen; i++) {
        int hi = hex_nibble(hex[i*2]);
        int lo = hex_nibble(hex[i*2+1]);
        if (hi < 0 || lo < 0) { uart_puts("error: bad hex\n"); return; }
        anl_buf[i] = (uint8_t)((hi << 4) | lo);
    }

    printk("loaded %zu bytes, running '%s'...\n", binlen, name);
    int ret = anl_load(name, anl_buf, binlen);
    printk("anl_load returned %d\n", ret);
}

/* fork example: child process entry */
struct fork_arg {
    int id;
    int iterations;
};

static void *fork_child_main(void *arg)
{
    struct fork_arg *fa = (struct fork_arg *)arg;
    int id = fa->id;
    int iters = fa->iterations;

    printk("[child %d] started, PID=%d, parent PID=%d\n",
           id,
           process_current()->pid,
           process_current()->parent ? process_current()->parent->pid : 0);

    for (int i = 0; i < iters; i++) {
        printk("[child %d] iteration %d/%d\n", id, i + 1, iters);
        k_msleep(500);
    }

    printk("[child %d] done\n", id);
    return (void *)(intptr_t)id;
}

/* fork <nchildren> [iterations] */
static void cmd_fork(char *args)
{
    int nchildren = 1;
    int iterations = 3;

    if (args && args[0]) {
        nchildren = atoi(args);
        char *sp = strchr(args, ' ');
        if (sp) iterations = atoi(sp + 1);
    }

    if (nchildren < 1 || nchildren > 4) {
        uart_puts("usage: fork <nchildren 1-4> [iterations]\n");
        return;
    }

    printk("[parent] PID=%d, forking %d child(ren), %d iterations each\n",
           process_current()->pid, nchildren, iterations);

    /* Static arg storage — one per possible child */
    static struct fork_arg fork_args[4];
    pid_t pids[4];

    for (int i = 0; i < nchildren; i++) {
        fork_args[i].id = i + 1;
        fork_args[i].iterations = iterations;
        char name[16];
        snprintf(name, sizeof(name), "child_%d", i + 1);
        pids[i] = new_task(name, fork_child_main, &fork_args[i]);
        if (pids[i] < 0) {
            printk("[parent] failed to fork child %d: %d\n", i + 1, pids[i]);
        } else {
            printk("[parent] forked child %d with PID=%d\n", i + 1, pids[i]);
        }
    }

    /* Wait for all children */
    for (int i = 0; i < nchildren; i++) {
        if (pids[i] < 0) continue;
        int status = 0;
        pid_t ret = waitpid(pids[i], &status, 0);
        printk("[parent] child PID=%d exited with status=%d\n", ret, status);
    }

    printk("[parent] all children done\n");
}

/* ps: list processes */
static void cmd_ps(void)
{
    printk("PID  PPID  NAME\n");
    for (int i = 0; i < CONFIG_MAX_PROCESS_COUNT; i++) {
        struct z_process *proc = process_get(i);
        if (proc && proc->pid > 0) {
            printk("%-4d %-4d  (pid slot %d)\n",
                   proc->pid,
                   proc->parent ? proc->parent->pid : 0,
                   i);
        }
    }
}

int main(void)
{
    uart_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_console));
    if (!device_is_ready(uart_dev)) {
        printk("UART not ready\n");
        return -1;
    }

    uart_puts("ANL loader + fork demo ready.\n");
    uart_puts("Commands: load <name> <hexdata> | fork [nchildren] [iters] | ps\n");
    uart_puts("anl> ");

    static char line[8192 + 64];
    while (1) {
        int len = readline(line, sizeof(line));
        if (len == 0) { uart_puts("anl> "); continue; }

        if (strncmp(line, "load ", 5) == 0)
            cmd_load(line + 5);
        else if (strncmp(line, "fork", 4) == 0)
            cmd_fork(line[4] == ' ' ? line + 5 : "");
        else if (strcmp(line, "ps") == 0)
            cmd_ps();
        else
            uart_puts("unknown command\n");

        uart_puts("anl> ");
    }
    return 0;
}
