#include <stdint.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/time_units.h>

#define TEST_PERIOD_US 1000U

static struct k_timer latency_timer;
static struct k_sem latency_sem;

static void timer_expiry_fn(struct k_timer *timer)
{
	ARG_UNUSED(timer);
	k_sem_give(&latency_sem);
}

static uint32_t cycles_to_ns_u32(uint32_t cycles)
{
	uint64_t ns = k_cyc_to_ns_floor64(cycles);

	if (ns > UINT32_MAX) {
		return UINT32_MAX;
	}

	return (uint32_t)ns;
}

int main(void)
{
	uint32_t period_ticks = k_us_to_ticks_ceil32(TEST_PERIOD_US);
	uint32_t start_cycle;
	uint32_t expected_cycle;
	uint32_t actual_cycle;
	uint32_t delta_cycles;
	uint32_t start_ticks;
	uint32_t expected_ticks;
	uint32_t now_ticks;
	int32_t tick_jitter;

	k_sem_init(&latency_sem, 0, 1);
	k_timer_init(&latency_timer, timer_expiry_fn, NULL);

	printk("one-shot cyclictest-style IRQ wakeup latency\n");
	printk("cpu cycles per sec: %u\n", sys_clock_hw_cycles_per_sec());
	printk("test period: %u us, period ticks: %u\n", TEST_PERIOD_US, period_ticks);

	start_cycle = k_cycle_get_32();
	start_ticks = k_uptime_ticks();
	expected_cycle = start_cycle + k_us_to_cyc_ceil32(TEST_PERIOD_US);
	expected_ticks = start_ticks + period_ticks;

	k_timer_start(&latency_timer, K_USEC(TEST_PERIOD_US), K_NO_WAIT);
	k_sem_take(&latency_sem, K_FOREVER);

	actual_cycle = k_cycle_get_32();
	delta_cycles = actual_cycle - expected_cycle;
	now_ticks = k_uptime_ticks();
	tick_jitter = (int32_t)(now_ticks - expected_ticks);

	printk("wakeup latency=%u cyc (%u ns) tick_jitter=%d\n",
	       delta_cycles,
	       cycles_to_ns_u32(delta_cycles),
	       tick_jitter);

	return 0;
}
