#include <stdint.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/time_units.h>

#define TEST_PERIOD_US      1000U
#define TEST_ITERATIONS     10000U
#define REPORT_EVERY        100U

static struct k_timer latency_timer;
static struct k_sem latency_sem;

static volatile uint32_t expected_cycle;
static volatile uint32_t expected_ticks;

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
	uint32_t min_cycles = UINT32_MAX;
	uint32_t max_cycles = 0U;
	uint64_t sum_cycles = 0U;
	uint32_t period_ticks = k_us_to_ticks_ceil32(TEST_PERIOD_US);

	k_sem_init(&latency_sem, 0, 1);
	k_timer_init(&latency_timer, timer_expiry_fn, NULL);

	printk("cyclictest-style IRQ wakeup latency test\n");
	printk("cpu cycles per sec: %u\n", sys_clock_hw_cycles_per_sec());
	printk("test period: %u us, period ticks: %u\n", TEST_PERIOD_US, period_ticks);
	printk("iterations: %u\n", TEST_ITERATIONS);

	for (uint32_t i = 1; i <= TEST_ITERATIONS; i++) {
		uint32_t start_cycle = k_cycle_get_32();
		uint32_t start_ticks = k_uptime_ticks();
		uint32_t actual_cycle;
		uint32_t delta_cycles;

		expected_cycle = start_cycle + k_us_to_cyc_ceil32(TEST_PERIOD_US);
		expected_ticks = start_ticks + period_ticks;

		k_timer_start(&latency_timer, K_USEC(TEST_PERIOD_US), K_NO_WAIT);
		k_sem_take(&latency_sem, K_FOREVER);

		actual_cycle = k_cycle_get_32();
		delta_cycles = actual_cycle - expected_cycle;

		if (delta_cycles < min_cycles) {
			min_cycles = delta_cycles;
		}
		if (delta_cycles > max_cycles) {
			max_cycles = delta_cycles;
		}
		sum_cycles += delta_cycles;

		if ((i % REPORT_EVERY) == 0U) {
			uint32_t avg_cycles = (uint32_t)(sum_cycles / i);
			uint32_t now_ticks = k_uptime_ticks();
			int32_t tick_jitter = (int32_t)(now_ticks - expected_ticks);

			printk("[%u] cur=%u cyc (%u ns) min=%u max=%u avg=%u tick_jitter=%d\n",
			       i,
			       delta_cycles,
			       cycles_to_ns_u32(delta_cycles),
			       min_cycles,
			       max_cycles,
			       avg_cycles,
			       tick_jitter);
		}
	}

	printk("done: min=%u cyc (%u ns), max=%u cyc (%u ns), avg=%u cyc (%u ns)\n",
	       min_cycles,
	       cycles_to_ns_u32(min_cycles),
	       max_cycles,
	       cycles_to_ns_u32(max_cycles),
	       (uint32_t)(sum_cycles / TEST_ITERATIONS),
	       cycles_to_ns_u32((uint32_t)(sum_cycles / TEST_ITERATIONS)));

	return 0;
}
