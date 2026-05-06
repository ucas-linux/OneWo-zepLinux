#include <stdint.h>
#include <zephyr/device.h>
#include <zephyr/drivers/counter.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/time_units.h>

#define TIMER_NODE DT_NODELABEL(timers2)
#define COUNTER_NODE DT_CHILD(TIMER_NODE, counter)

#if !DT_NODE_HAS_STATUS(COUNTER_NODE, okay)
#error "TIM2 counter node is not enabled"
#endif

#define TEST_PERIOD_US   1000U
#define TEST_ITERATIONS  100U
#define REPORT_EVERY     10U
#define ALARM_CHANNEL_ID 0U

static const struct device *const counter_dev = DEVICE_DT_GET(COUNTER_NODE);
static struct k_sem sample_sem;

static volatile uint32_t expected_counter_tick;
static volatile uint32_t expected_cycle;
static volatile uint32_t measured_cycle;
static volatile uint32_t measured_counter_tick;
static volatile uint32_t sample_ready;

static void alarm_callback(const struct device *dev, uint8_t chan_id,
			   uint32_t ticks, void *user_data)
{
	ARG_UNUSED(dev);
	ARG_UNUSED(chan_id);
	ARG_UNUSED(user_data);

	measured_cycle = k_cycle_get_32();
	measured_counter_tick = ticks;
	sample_ready = 1U;
	k_sem_give(&sample_sem);
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
	struct counter_alarm_cfg alarm_cfg = {
		.callback = alarm_callback,
		.user_data = NULL,
		.flags = COUNTER_ALARM_CFG_ABSOLUTE | COUNTER_ALARM_CFG_EXPIRE_WHEN_LATE,
	};
	uint32_t min_cycles = UINT32_MAX;
	uint32_t max_cycles = 0U;
	uint64_t sum_cycles = 0U;
	uint32_t period_ticks;
	int err;

	if (!device_is_ready(counter_dev)) {
		printk("counter device not ready\n");
		return 0;
	}

	period_ticks = counter_us_to_ticks(counter_dev, TEST_PERIOD_US);
	if (period_ticks == 0U) {
		period_ticks = 1U;
	}

	k_sem_init(&sample_sem, 0, 1);

	err = counter_start(counter_dev);
	if (err < 0 && err != -EALREADY) {
		printk("counter_start failed: %d\n", err);
		return 0;
	}

	printk("hardware timer ISR latency test (100 samples)\n");
	printk("counter freq: %u Hz\n", counter_get_frequency(counter_dev));
	printk("cpu cycles per sec: %u\n", sys_clock_hw_cycles_per_sec());
	printk("test period: %u us, counter ticks: %u\n", TEST_PERIOD_US, period_ticks);
	printk("iterations: %u\n", TEST_ITERATIONS);

	for (uint32_t i = 1; i <= TEST_ITERATIONS; i++) {
		uint32_t now_tick;
		uint32_t delta_cycles;
		uint32_t isr_to_expected_ticks;

		err = counter_get_value(counter_dev, &now_tick);
		if (err < 0) {
			printk("counter_get_value failed: %d\n", err);
			break;
		}

		expected_counter_tick = now_tick + period_ticks;
		expected_cycle = k_cycle_get_32() + k_us_to_cyc_ceil32(TEST_PERIOD_US);
		sample_ready = 0U;
		alarm_cfg.ticks = expected_counter_tick;

		err = counter_set_channel_alarm(counter_dev, ALARM_CHANNEL_ID, &alarm_cfg);
		if (err < 0) {
			printk("counter_set_channel_alarm failed: %d\n", err);
			break;
		}

		k_sem_take(&sample_sem, K_FOREVER);
		if (!sample_ready) {
			printk("sample not ready\n");
			break;
		}

		delta_cycles = measured_cycle - expected_cycle;
		isr_to_expected_ticks = measured_counter_tick - expected_counter_tick;

		if (delta_cycles < min_cycles) {
			min_cycles = delta_cycles;
		}
		if (delta_cycles > max_cycles) {
			max_cycles = delta_cycles;
		}
		sum_cycles += delta_cycles;

		if ((i % REPORT_EVERY) == 0U) {
			uint32_t avg_cycles = (uint32_t)(sum_cycles / i);

			printk("[%u] cur=%u cyc (%u ns) min=%u max=%u avg=%u counter_jitter=%u\n",
			       i,
			       delta_cycles,
			       cycles_to_ns_u32(delta_cycles),
			       min_cycles,
			       max_cycles,
			       avg_cycles,
			       isr_to_expected_ticks);
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
