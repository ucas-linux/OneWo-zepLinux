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

/* Ring buffer for UART RX data */
#define RX_RINGBUF_SIZE 256
static uint8_t rx_ringbuf[RX_RINGBUF_SIZE];
static volatile uint16_t rx_head = 0;
static volatile uint16_t rx_tail = 0;

/* Forward declarations */
static void uart_puts(const char *s);

/* UART interrupt callback - detects Ctrl+D and sends signal to foreground process */
static void uart_isr_callback(const struct device *dev, void *user_data)
{
    ARG_UNUSED(user_data);

    if (!uart_irq_update(dev)) {
        return;
    }

    if (!uart_irq_rx_ready(dev)) {
        return;
    }

    uint8_t c;
    while (uart_fifo_read(dev, &c, 1) == 1) {
        if (c == 4) {  /* Ctrl+D (ASCII 4) */
            uart_puts("^D\n");
            uart_puts("\n");
            uart_puts("╔════════════════════════════════════════════════════╗\n");
            uart_puts("║       PER-PROCESS SIGNAL PROPAGATION TRACE        ║\n");
            uart_puts("╚════════════════════════════════════════════════════╝\n");
            uart_puts("[ISR] Step 1: Ctrl+D (ASCII 4) detected in UART interrupt handler\n");
            uart_puts("[ISR] Step 2: Running in interrupt context (hardware triggered)\n");

            /* Get foreground process group */
            pid_t fg_pgid = signal_get_foreground_pgid();
            uart_puts("[ISR] Step 3: Query foreground process group ID\n");

            if (fg_pgid > 0) {
                uart_puts("[ISR] Step 4: Foreground process found (PID=");
                char buf[16];
                snprintf(buf, sizeof(buf), "%d", fg_pgid);
                uart_puts(buf);
                uart_puts(")\n");

                uart_puts("[ISR] Step 5: Sending SIGINT (signal 2) to foreground process...\n");
                /* Send SIGINT to foreground process only */
                int ret = kill(fg_pgid, SIGINT);

                if (ret == 0) {
                    uart_puts("[ISR] Step 6: Signal delivered successfully to process's event queue\n");
                    uart_puts("[ISR] Step 7: Only the targeted process will receive this signal\n");
                } else {
                    uart_puts("[ISR] Step 6: Failed to deliver signal (process may have exited)\n");
                }
            } else {
                uart_puts("[ISR] Step 4: No foreground process (signal ignored)\n");
            }

            uart_puts("[ISR] Step 8: Exiting interrupt handler\n");
            uart_puts("════════════════════════════════════════════════════\n");
            uart_puts("\n");
        } else {
            /* Store in ring buffer */
            uint16_t next_head = (rx_head + 1) % RX_RINGBUF_SIZE;
            if (next_head != rx_tail) {
                rx_ringbuf[rx_head] = c;
                rx_head = next_head;
            }
            /* Buffer full - drop character */
        }
    }
}

/* Signal handler for SIGINT */
static volatile bool sigint_received = false;

static void sigint_handler(int sig)
{
    sigint_received = true;
    printk("\n");
    printk("┌────────────────────────────────────────────────────┐\n");
    printk("│      PROCESS RECEIVED SIGNAL (PER-PROCESS)        │\n");
    printk("└────────────────────────────────────────────────────┘\n");
    printk("[HANDLER] Signal %d (SIGINT) handler invoked\n", sig);
    printk("[HANDLER] This is a per-process signal handler\n");
    printk("[HANDLER] Setting interrupt flag for this process\n");
    printk("────────────────────────────────────────────────────\n");
    printk("\n");
}

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

        /* Wait for data in ring buffer */
        while (rx_head == rx_tail) {
            k_sleep(K_MSEC(1));
        }

        /* Get character from ring buffer */
        c = rx_ringbuf[rx_tail];
        rx_tail = (rx_tail + 1) % RX_RINGBUF_SIZE;

        if (c == '\r' || c == '\n') {
            buf[i] = '\0';
            uart_puts("\r\n");
            return i;
        }
        if (c == '\b' || c == 127) {
            if (i > 0) { i--; uart_puts("\b \b"); }
            continue;
        }
        /* Skip Ctrl+D in readline */
        if (c == 4) {
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

    struct z_process *proc = process_current();

    printk("[CHILD %d] Started, PID=%d, parent PID=%d\n",
           id, proc->pid,
           proc->parent ? proc->parent->pid : 0);
    printk("[CHILD %d] Will run %d iterations\n", id, iters);
    printk("[CHILD %d] Has own signal state at 0x%p\n", id, proc->signal_state);
    printk("[CHILD %d] Listening for per-process signals...\n", id);

    /* Set up signal handler for this child */
    sigint_received = false;
    signal(SIGINT, sigint_handler);

    for (int i = 0; i < iters; i++) {
        /* Wait for signal with timeout (per-process) */
        int sig = signal_wait(K_MSEC(500));

        if (sig == SIGINT) {
            printk("\n");
            printk("[CHILD %d] ⚠️  SIGINT received at iteration %d/%d\n",
                   id, i + 1, iters);
            printk("[CHILD %d] signal_wait() detected signal %d (per-process)\n", id, sig);
            printk("[CHILD %d] Exiting due to interrupt signal\n", id);
            printk("\n");
            return (void *)(intptr_t)-1;
        }

        /* Check if signal handler was called */
        if (sigint_received) {
            printk("[CHILD %d] ⚠️  sigint_received flag set at iteration %d/%d\n", id, i + 1, iters);
            printk("[CHILD %d] Exiting due to signal handler\n", id);
            return (void *)(intptr_t)-1;
        }

        /* Check for any pending signals */
        signal_check_pending();

        printk("[CHILD %d] Iteration %d/%d\n", id, i + 1, iters);
    }

    printk("[CHILD %d] ✓ Completed all %d iterations\n", id, iters);
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

    struct z_process *parent_proc = process_current();

    printk("\n");
    printk("========================================\n");
    printk("  Multi-Process Test with Per-Process Signals\n");
    printk("========================================\n");
    printk("[PARENT] PID=%d\n", parent_proc->pid);
    printk("[PARENT] Forking %d child process(es)\n", nchildren);
    printk("[PARENT] Each child runs %d iterations\n", iterations);
    printk("[PARENT] Each child has independent signal state\n");
    printk("========================================\n");
    printk("\n");

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
            printk("[PARENT] ❌ Failed to fork child %d: error %d\n", i + 1, pids[i]);
        } else {
            printk("[PARENT] ✓ Forked child %d with PID=%d\n", i + 1, pids[i]);
        }
    }

    printk("[PARENT] All children forked, now waiting...\n");
    printk("\n");

    /* Wait for all children */
    for (int i = 0; i < nchildren; i++) {
        if (pids[i] < 0) continue;

        printk("[PARENT] Waiting for child %d (PID=%d)...\n", i + 1, pids[i]);

        int status = 0;
        pid_t ret = waitpid(pids[i], &status, 0);

        if (ret > 0) {
            printk("[PARENT] ✓ Child PID=%d exited with status=%d\n", ret, status);
        }
    }

    printk("\n");
    printk("========================================\n");
    printk("[PARENT] ✓ All children completed\n");
    printk("========================================\n");
    printk("\n");
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

/* loop: infinite loop that prints counter - must use Ctrl+D to stop */
static void cmd_loop(void)
{
    int counter = 0;

    struct z_process *proc = process_current();

    printk("\n");
    printk("========================================\n");
    printk("  Testing Per-Process Signal on Ctrl+D\n");
    printk("========================================\n");
    printk("[LOOP] Starting infinite loop...\n");
    printk("[LOOP] Process PID=%d\n", proc->pid);
    printk("[LOOP] Signal state at 0x%p\n", proc->signal_state);
    printk("[LOOP] Setting this process as foreground\n");

    /* Set this process as foreground to receive Ctrl+D signal */
    signal_set_foreground_pgid(proc->pid);

    /* Install signal handler */
    sigint_received = false;
    signal(SIGINT, sigint_handler);

    printk("[LOOP] SIGINT handler installed\n");
    printk("[LOOP] Press Ctrl+D to stop this process only\n");
    printk("========================================\n");
    printk("\n");

    while (1) {
        printk("[LOOP] Iteration %d (waiting for per-process signal...)\n", counter++);

        /* Wait for signal with timeout (per-process event) */
        int sig = signal_wait(K_MSEC(500));

        if (sig == SIGINT) {
            printk("\n");
            printk("┌────────────────────────────────────────────────────┐\n");
            printk("│   SIGNAL RECEIVED IN LOOP (PER-PROCESS)           │\n");
            printk("└────────────────────────────────────────────────────┘\n");
            printk("[LOOP] Step 9: signal_wait() returned signal %d\n", sig);
            printk("[LOOP] Step 10: This signal was sent ONLY to PID=%d\n", proc->pid);
            printk("[LOOP] Step 11: Other processes are NOT affected\n");
            printk("[LOOP] Step 12: Breaking loop due to per-process signal\n");
            printk("────────────────────────────────────────────────────\n");
            printk("\n");
            break;
        }

        if (sig == 0) {
            printk("[LOOP] No signal received, timeout occurred\n");
        }

        /* Check if signal handler was called */
        if (sigint_received) {
            printk("[LOOP] sigint_received flag set by handler, exiting\n");
            break;
        }

        /* Check for any pending signals */
        signal_check_pending();
    }

    /* Clear foreground when done */
    signal_set_foreground_pgid(0);

    printk("\n");
    printk("========================================\n");
    printk("[LOOP] Stopped after %d iterations\n", counter);
    printk("[LOOP] Reason: Per-process SIGINT received\n");
    printk("[LOOP] Foreground cleared\n");
    printk("========================================\n");
    printk("\n");
}

int main(void)
{
    uart_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_console));
    if (!device_is_ready(uart_dev)) {
        printk("UART not ready\n");
        return -1;
    }

    /* Initialize signal state for main process */
    struct z_process *main_proc = process_current();
    if (main_proc) {
        main_proc->signal_state = k_malloc(sizeof(struct process_signal));
        if (main_proc->signal_state) {
            signal_process_init(main_proc);
            printk("[INIT] Main process PID=%d signal state initialized\n", main_proc->pid);
        }
    }

    printk("\n");
    printk("========================================\n");
    printk("  Per-Process Signal Handler (Linux-like)\n");
    printk("========================================\n");
    printk("Mode: Per-process signal delivery\n");
    printk("Main process: PID=%d\n", main_proc ? main_proc->pid : 0);
    printk("Signal state: 0x%p\n", main_proc ? main_proc->signal_state : NULL);
    printk("Ring buffer: %d bytes at 0x%p\n", RX_RINGBUF_SIZE, rx_ringbuf);
    printk("========================================\n");
    printk("\n");

    /* Enable UART interrupt callback */
    uart_irq_callback_user_data_set(uart_dev, uart_isr_callback, NULL);
    uart_irq_rx_enable(uart_dev);

    printk("[INIT] UART interrupt handler registered\n");
    printk("[INIT] UART RX interrupt enabled\n");
    printk("[INIT] Ctrl+D will send SIGINT to foreground process only\n");
    printk("\n");

    uart_puts("ANL loader + fork demo ready (per-process signal delivery).\n");
    uart_puts("Commands: load <name> <hexdata> | fork [nchildren] [iters] | loop | ps\n");
    uart_puts("Press Ctrl+D to interrupt foreground process only (Linux-like)\n");
    uart_puts("anl> ");

    static char line[8192 + 64];
    while (1) {
        int len = readline(line, sizeof(line));
        if (len == 0) { uart_puts("anl> "); continue; }

        printk("\n[SHELL] Command received: '%s' (length=%d)\n", line, len);

        if (strncmp(line, "load ", 5) == 0)
            cmd_load(line + 5);
        else if (strncmp(line, "fork", 4) == 0)
            cmd_fork(line[4] == ' ' ? line + 5 : "");
        else if (strcmp(line, "loop") == 0)
            cmd_loop();
        else if (strcmp(line, "ps") == 0)
            cmd_ps();
        else {
            uart_puts("unknown command\n");
            printk("[SHELL] Unknown command: '%s'\n", line);
        }

        printk("[SHELL] Command completed, returning to prompt\n");
        uart_puts("anl> ");
    }
    return 0;
}
