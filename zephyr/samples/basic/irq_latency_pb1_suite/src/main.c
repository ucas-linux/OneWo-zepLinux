#include <stdint.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/counter.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/time_units.h>

#define TIMER_NODE DT_NODELABEL(timers2)
#define COUNTER_NODE DT_CHILD(TIMER_NODE, counter)
#define LOOPBACK_OUT_NODE DT_ALIAS(loopback_out)
#define LOOPBACK_IN_NODE  DT_ALIAS(loopback_in)

#if !DT_NODE_HAS_STATUS(COUNTER_NODE, okay)
#error "TIM2 counter node is not enabled"
#endif

#if !DT_NODE_HAS_STATUS(LOOPBACK_OUT_NODE, okay)
#error "Unsupported board: loopback_out devicetree alias is not defined"
#endif

#if !DT_NODE_HAS_STATUS(LOOPBACK_IN_NODE, okay)
#error "Unsupported board: loopback_in devicetree alias is not defined"
#endif

#define TEST_PERIOD_US   1000U
#define TEST_ITERATIONS  20U
#define ALARM_CHANNEL_ID 0U

static const struct device *const counter_dev = DEVICE_DT_GET(COUNTER_NODE);
static const struct gpio_dt_spec loopback_out = GPIO_DT_SPEC_GET(LOOPBACK_OUT_NODE, gpios);
static const struct gpio_dt_spec loopback_in = GPIO_DT_SPEC_GET(LOOPBACK_IN_NODE, gpios);
static struct k_sem sample_sem;
static struct k_timer wake_timer;

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

static void timer_expiry_fn(struct k_timer *timer)
{
	ARG_UNUSED(timer);
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

static int run_gpio_check(int level)
{
	int ret;

	ret = gpio_pin_set_dt(&loopback_out, level);
	if (ret < 0) {
		printk("Failed to drive output: %d\n", ret);
		return ret;
	}

	k_msleep(1);

	ret = gpio_pin_get_dt(&loopback_in);
	if (ret < 0) {
		printk("Failed to sample input: %d\n", ret);
		return ret;
	}

	printk("loopback drive=%d sample=%d %s\n",
	       level, ret, (ret == level) ? "PASS" : "FAIL");

	return (ret == level) ? 0 : -EIO;
}

int main(void)
{
	struct counter_alarm_cfg alarm_cfg = {
		.callback = alarm_callback,
		.user_data = NULL,
		.flags = COUNTER_ALARM_CFG_ABSOLUTE | COUNTER_ALARM_CFG_EXPIRE_WHEN_LATE,
	};
	uint32_t period_ticks;
	uint64_t sum_isr_cycles = 0U;
	uint64_t sum_wakeup_cycles = 0U;
	int err;
	int ret;

	if (!device_is_ready(counter_dev)) {
		printk("counter device not ready\n");
		return 0;
	}

	if (!gpio_is_ready_dt(&loopback_out) || !gpio_is_ready_dt(&loopback_in)) {
		printk("GPIO device not ready\n");
		return 0;
	}

	ret = gpio_pin_configure_dt(&loopback_out, GPIO_OUTPUT_INACTIVE);
	if (ret < 0) {
		printk("Failed to configure output: %d\n", ret);
		return 0;
	}

	ret = gpio_pin_configure_dt(&loopback_in, GPIO_INPUT);
	if (ret < 0) {
		printk("Failed to configure input: %d\n", ret);
		return 0;
	}

	period_ticks = counter_us_to_ticks(counter_dev, TEST_PERIOD_US);
	if (period_ticks == 0U) {
		period_ticks = 1U;
	}

	k_sem_init(&sample_sem, 0, 1);
	k_timer_init(&wake_timer, timer_expiry_fn, NULL);

	err = counter_start(counter_dev);
	if (err < 0 && err != -EALREADY) {
		printk("counter_start failed: %d\n", err);
		return 0;
	}

	printk("PB1->PB2 suite: two GPIO checks, %u ISR samples, %u wakeup samples\n",
	       TEST_ITERATIONS, TEST_ITERATIONS);
	printk("counter freq: %u Hz\n", counter_get_frequency(counter_dev));
	printk("cpu cycles per sec: %u\n", sys_clock_hw_cycles_per_sec());

	run_gpio_check(0);
	run_gpio_check(1);

	for (uint32_t i = 1; i <= TEST_ITERATIONS; i++) {
		uint32_t now_tick;
		uint32_t isr_cycles;
		uint32_t wake_expected;
		uint32_t wake_actual;
		uint32_t wake_cycles;

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

		isr_cycles = measured_cycle - expected_cycle;
		sum_isr_cycles += isr_cycles;

		wake_expected = k_cycle_get_32() + k_us_to_cyc_ceil32(TEST_PERIOD_US);
		k_timer_start(&wake_timer, K_USEC(TEST_PERIOD_US), K_NO_WAIT);
		k_sem_take(&sample_sem, K_FOREVER);
		wake_actual = k_cycle_get_32();
		wake_cycles = wake_actual - wake_expected;
		sum_wakeup_cycles += wake_cycles;

		printk("[%u] isr=%u cyc (%u ns) wake=%u cyc (%u ns)\n",
		       i,
		       isr_cycles,
		       cycles_to_ns_u32(isr_cycles),
		       wake_cycles,
		       cycles_to_ns_u32(wake_cycles));
	}

	printk("suite done: avg_isr=%u cyc (%u ns), avg_wake=%u cyc (%u ns)\n",
	       (uint32_t)(sum_isr_cycles / TEST_ITERATIONS),
	       cycles_to_ns_u32((uint32_t)(sum_isr_cycles / TEST_ITERATIONS)),
	       (uint32_t)(sum_wakeup_cycles / TEST_ITERATIONS),
	       cycles_to_ns_u32((uint32_t)(sum_wakeup_cycles / TEST_ITERATIONS)));

	return 0;
}
