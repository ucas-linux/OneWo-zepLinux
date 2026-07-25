#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/uart.h>
#include <string.h>
#include <stdlib.h>
#include "anl_loader.h"

static const struct device *uart_dev;

/* Exported symbols available to loaded ANL modules */
static void anl_printk_wrapper(const char *fmt)
{
    printk("%s", fmt);
}

const struct anl_export _anl_exports[] = {
    { "printk", (uintptr_t)anl_printk_wrapper },
    { "k_msleep", (uintptr_t)k_msleep },
};
const int _anl_exports_count = 2;

static uint8_t anl_buf[8192];

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

int main(void)
{
    uart_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_console));
    if (!device_is_ready(uart_dev)) {
        printk("UART not ready\n");
        return -1;
    }

    uart_puts("ANL loader ready. Commands: load <name> <hexdata>\nanl> ");

    static char line[8192 + 64];
    while (1) {
        int len = readline(line, sizeof(line));
        if (len == 0) { uart_puts("anl> "); continue; }

        if (strncmp(line, "load ", 5) == 0)
            cmd_load(line + 5);
        else
            uart_puts("unknown command\n");

        uart_puts("anl> ");
    }
    return 0;
}
