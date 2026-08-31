#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/kernel/process.h>
#include <string.h>
#include <stdlib.h>
#include "anl_loader.h"
#include "shell_process.h"
#include "signal.h"

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

        /* Handle Ctrl+C (0x03) - send SIGINT to foreground process */
        if (c == 0x03) {
            uart_puts("^C\r\n");
            pid_t fg_pgid = signal_get_foreground_pgid();
            if (fg_pgid > 0) {
                printk("[Shell] Sending SIGINT to PID %d\n", fg_pgid);
                kill(fg_pgid, SIGINT);
            }
            buf[i] = '\0';
            return i;
        }

        /* Handle Ctrl+D (0x04) - send SIGTSTP to foreground process */
        if (c == 0x04) {
            uart_puts("^D\r\n");
            pid_t fg_pgid = signal_get_foreground_pgid();
            if (fg_pgid > 0) {
                printk("[Shell] Sending SIGTSTP (suspend) to PID %d\n", fg_pgid);
                kill(fg_pgid, SIGTSTP);
            } else {
                /* No foreground process - EOF behavior */
                buf[i] = '\0';
                return i;
            }
            buf[i] = '\0';
            return i;
        }

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
    printk("PID  PPID  STATE       NAME\n");
    for (int i = 0; i < CONFIG_MAX_PROCESS_COUNT; i++) {
        struct z_process *proc = process_get(i);
        if (proc && proc->pid > 0) {
            const char *state = "running";
            if (signal_is_suspended(proc->pid) > 0) {
                state = "suspended";
            }
            printk("%-4d %-4d  %-10s  (pid slot %d)\n",
                   proc->pid,
                   proc->parent ? proc->parent->pid : 0,
                   state,
                   i);
        }
    }

    pid_t suspended_pid = signal_get_suspended_fg_pid();
    if (suspended_pid > 0) {
        printk("\nSuspended foreground process: %d (use 'fg' to resume)\n", suspended_pid);
    }
}

/* fg: resume suspended foreground process */
static void cmd_fg(void)
{
    pid_t suspended_pid = signal_get_suspended_fg_pid();

    if (suspended_pid <= 0) {
        uart_puts("No suspended foreground process\n");
        return;
    }

    printk("Resuming process PID=%d...\n", suspended_pid);
    int ret = signal_resume_process(suspended_pid);

    if (ret == 0) {
        printk("Process %d resumed and brought to foreground\n", suspended_pid);
    } else {
        printk("Failed to resume process %d: %d\n", suspended_pid, ret);
    }
}

/* loop: test command that loops and can be suspended */
static void *loop_thread(void *arg)
{
    int duration = (int)(intptr_t)arg;
    struct z_process *proc = process_current();

    if (duration <= 0) {
        printk("[loop] Started in PID=%d, infinite loop\n", proc->pid);
    } else {
        printk("[loop] Started in PID=%d, will loop %d times\n", proc->pid, duration);
    }
    printk("[loop] Press Ctrl+D to suspend, or Ctrl+C to interrupt\n");

    /* Set this process as foreground */
    signal_set_foreground_pgid(proc->pid);

    /* Install signal handlers */
    volatile int interrupted = 0;
    void sigint_handler(int sig) {
        interrupted = 1;
        printk("\n[loop] Interrupted by SIGINT\n");
    }
    signal(SIGINT, sigint_handler);

    /* Infinite or counted loop with CPU-intensive work to slow down QEMU */
    int count = 0;
    while (!interrupted) {
        if (duration > 0 && count >= duration) {
            break;
        }

        if (count % 100 == 0) {
            printk(".");  /* Print dot every 100 iterations */
        }

        /* Do some CPU work to consume time even in QEMU */
        volatile int dummy = 0;
        for (int i = 0; i < 10000; i++) {
            dummy += i * i;
        }

        /* Check for pending signals frequently */
        signal_check_pending();

        count++;
    }

    if (!interrupted) {
        printk("\n[loop] Completed normally after %d iterations\n", count);
    }

    /* Clear foreground */
    signal_set_foreground_pgid(0);

    return (void *)(intptr_t)0;
}

static void cmd_loop(char *args)
{
    int duration = 1000;  /* default 1000 iterations */

    if (args && args[0]) {
        duration = atoi(args);
        if (duration <= 0) duration = 1000;
    }

    /* Create a new process for the loop */
    pid_t pid = new_task("loop_test", loop_thread, (void *)(intptr_t)duration);

    if (pid < 0) {
        printk("Failed to create loop process: %d\n", pid);
        return;
    }

    printk("Loop process started with PID=%d (running in background)\n", pid);
    /* Don't waitpid() here - let it run in background so shell can handle Ctrl+D */
}

int main(void)
{
    uart_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_console));
    if (!device_is_ready(uart_dev)) {
        printk("UART not ready\n");
        return -1;
    }

    uart_puts("ANL loader + fork demo ready.\n");
    uart_puts("Commands:\n");
    uart_puts("  load <name> <hexdata> - Load ANL module\n");
    uart_puts("  fork [nchildren] [iters] - Fork test\n");
    uart_puts("  loop [seconds] - Loop test (Ctrl+D to suspend, Ctrl+C to stop)\n");
    uart_puts("  fg - Resume suspended process\n");
    uart_puts("  ps - List processes\n");
    uart_puts("anl> ");

    static char line[8192 + 64];
    while (1) {
        int len = readline(line, sizeof(line));
        if (len == 0) { uart_puts("anl> "); continue; }

        if (strncmp(line, "load ", 5) == 0)
            cmd_load(line + 5);
        else if (strncmp(line, "fork", 4) == 0)
            cmd_fork(line[4] == ' ' ? line + 5 : "");
        else if (strncmp(line, "loop", 4) == 0)
            cmd_loop(line[4] == ' ' ? line + 5 : "");
        else if (strcmp(line, "ps") == 0)
            cmd_ps();
        else if (strcmp(line, "fg") == 0)
            cmd_fg();
        else
            uart_puts("unknown command\n");

        uart_puts("anl> ");
    }
    return 0;
}
