#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/init.h>
#include <zephyr/shell/shell.h>
#include <stdint.h>

static volatile uint32_t boot_mark_cycle;
static volatile uint32_t main_entry_cycle;

static uint32_t cycles_to_ns_u32(uint32_t cycles)
{
	uint64_t ns = k_cyc_to_ns_floor64(cycles);

	if (ns > UINT32_MAX) {
		return UINT32_MAX;
	}

	return (uint32_t)ns;
}

static int boot_time_mark(void)
{
	boot_mark_cycle = k_cycle_get_32();
	return 0;
}

SYS_INIT(boot_time_mark, PRE_KERNEL_1, 0);

/* Shell command: system info */
static int cmd_sysinfo(const struct shell *sh, size_t argc, char **argv)
{
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	shell_print(sh, "OneWo-zepLinux System Information");
	shell_print(sh, "==================================");
	shell_print(sh, "OS:           Zephyr RTOS");
	shell_print(sh, "Platform:     AS32X601 (RISC-V)");
	shell_print(sh, "CPU Freq:     %u Hz", sys_clock_hw_cycles_per_sec());
	shell_print(sh, "Uptime:       %lld ms", k_uptime_get());

	return 0;
}

/* Shell command: boot time */
static int cmd_boottime(const struct shell *sh, size_t argc, char **argv)
{
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	uint32_t boot_cycles = main_entry_cycle - boot_mark_cycle;
	uint32_t boot_ns = cycles_to_ns_u32(boot_cycles);
	uint32_t boot_us = boot_ns / 1000;

	shell_print(sh, "Boot Time Analysis");
	shell_print(sh, "==================");
	shell_print(sh, "Cycles:       %u", boot_cycles);
	shell_print(sh, "Time:         %u ns (%u us)", boot_ns, boot_us);

	return 0;
}

/* Shell command: version */
static int cmd_version(const struct shell *sh, size_t argc, char **argv)
{
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	shell_print(sh, "OneWo-zepLinux v1.0");
	shell_print(sh, "Ultra-fast RTOS for embedded systems");
	shell_print(sh, "Based on Zephyr RTOS");

	return 0;
}

/* Register shell commands */
SHELL_CMD_REGISTER(sysinfo, NULL, "Display system information", cmd_sysinfo);
SHELL_CMD_REGISTER(boottime, NULL, "Display boot time", cmd_boottime);
SHELL_CMD_REGISTER(version, NULL, "Display version information", cmd_version);

int main(void)
{
	main_entry_cycle = k_cycle_get_32();

	uint32_t boot_cycles = main_entry_cycle - boot_mark_cycle;
	uint32_t boot_ns = cycles_to_ns_u32(boot_cycles);
	uint32_t boot_us = boot_ns / 1000;

	printk("\n");
	printk("========================================\n");
	printk("  Welcome to OneWo-zepLinux\n");
	printk("========================================\n");
	printk("\n");
	printk("Boot Time: %u us (%u cycles)\n", boot_us, boot_cycles);
	printk("CPU:       %u Hz\n", sys_clock_hw_cycles_per_sec());
	printk("\n");
	printk("System ready. Type 'help' for commands.\n");
	printk("\n");

	return 0;
}
