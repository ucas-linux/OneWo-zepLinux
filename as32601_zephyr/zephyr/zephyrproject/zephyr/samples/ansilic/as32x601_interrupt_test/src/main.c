#include <stdint.h>
#include <inttypes.h>
#include <zephyr/kernel.h>
#include <zephyr/irq.h>
#include <zephyr/sys/printk.h>
#include <zephyr/arch/riscv/csr.h>
#include <zephyr/arch/riscv/irq.h>
#include <zephyr/sys_clock.h>
#include <zephyr/devicetree.h>

extern volatile uint32_t as32x601_timer_isr_count;

#define CLINT_NODE DT_NODELABEL(clint)
#define AS32X601_CLINT_MSIP_BASE DT_REG_ADDR_RAW(CLINT_NODE)
#define CLINT_MSIP (*(volatile uint32_t *)AS32X601_CLINT_MSIP_BASE)

static volatile uint32_t msoft_count;
static uint64_t prev_cycle_count;
static int64_t prev_uptime_ms;
static uint32_t prev_ticks;
static bool first_sample = true;

static void msoft_isr(const void *arg)
{
	ARG_UNUSED(arg);

	CLINT_MSIP = 0U;
	msoft_count++;
}

static void trigger_msoft(void)
{
	CLINT_MSIP = 1U;
}

static void print_timer_snapshot(void)
{
	uint64_t now_cycle_count = sys_clock_cycle_get_64();
	int64_t now_uptime_ms = k_uptime_get();
	uint32_t now_ticks = sys_clock_tick_get_32();
	uint64_t dcycles = 0U;
	int64_t duptime_ms = 0;
	uint32_t dticks = 0U;
	uint64_t timer_hz_est = 0U;
	uint64_t cycles_per_tick_est = 0U;
	uint32_t timer_isr_hits = as32x601_timer_isr_count;

	if (!first_sample) {
		dcycles = now_cycle_count - prev_cycle_count;
		duptime_ms = now_uptime_ms - prev_uptime_ms;
		dticks = now_ticks - prev_ticks;

		if (duptime_ms > 0) {
			timer_hz_est = (dcycles * 1000U) / (uint64_t)duptime_ms;
		}

		if (dticks > 0U) {
			cycles_per_tick_est = dcycles / dticks;
		}
	}

	printk("msoft=%u tisr=%u uptime_ms=%lld ticks=%u cycles=%llu dcycles=%llu dticks=%u duptime=%lldms hz_est=%llu cyc/tick=%llu mie=0x%08lx mip=0x%08lx\n",
	       msoft_count,
	       timer_isr_hits,
	       (long long)now_uptime_ms,
	       now_ticks,
	       (unsigned long long)now_cycle_count,
	       (unsigned long long)dcycles,
	       dticks,
	       (long long)duptime_ms,
	       (unsigned long long)timer_hz_est,
	       (unsigned long long)cycles_per_tick_est,
	       csr_read(mie), csr_read(mip));

	prev_cycle_count = now_cycle_count;
	prev_uptime_ms = now_uptime_ms;
	prev_ticks = now_ticks;
	first_sample = false;
}

int main(void)
{
	IRQ_CONNECT(RISCV_IRQ_MSOFT, 0, msoft_isr, NULL, 0);
	irq_enable(RISCV_IRQ_MSOFT);

	printk("AS32X601 Zephyr-style interrupt test start\n");
	printk("expect: uptime~1000ms, ticks~%u/s, cycles~180000000/s, cyc/tick~%u\n",
	       CONFIG_SYS_CLOCK_TICKS_PER_SEC,
	       180000000U / CONFIG_SYS_CLOCK_TICKS_PER_SEC);

	while (1) {
		trigger_msoft();
		k_busy_wait(100);
		print_timer_snapshot();
		k_msleep(1000);
	}

	return 0;
}
