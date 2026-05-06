#include <stdint.h>
#include <errno.h>
#include <stdarg.h>

#include <zephyr/devicetree.h>
#include <zephyr/drivers/counter.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/time_units.h>

#include "devfd.h"

#define open  dev_open
#define close dev_close
#define ioctl dev_ioctl

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
#define ALARM_CHANNEL_ID 0U

static struct k_sem sample_sem;

static volatile uint32_t expected_counter_tick;
static volatile uint32_t expected_cycle;
static volatile uint32_t measured_cycle;
static volatile uint32_t measured_counter_tick;
static volatile uint32_t sample_ready;

static uint32_t cycles_to_ns_u32(uint32_t cycles)
{
	uint64_t ns = k_cyc_to_ns_floor64(cycles);

	if (ns > UINT32_MAX) {
		return UINT32_MAX;
	}

	return (uint32_t)ns;
}

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

int main(void)
{
	struct counter_alarm_cfg alarm_cfg = {
		.callback = alarm_callback,
		.user_data = NULL,
		.flags = COUNTER_ALARM_CFG_ABSOLUTE | COUNTER_ALARM_CFG_EXPIRE_WHEN_LATE,
	};
	struct gpio_ioctl_configure out_cfg = {
		.flags = GPIO_OUTPUT_INACTIVE,
	};
	struct gpio_ioctl_configure in_cfg = {
		.flags = GPIO_INPUT,
	};
	struct gpio_ioctl_set set_high = {
		.value = 1,
	};
	struct counter_ioctl_get_value get_value = {
		.ticks = (uint32_t *)&measured_counter_tick,
	};
	struct counter_ioctl_set_alarm set_alarm = {
		.chan_id = ALARM_CHANNEL_ID,
		.alarm_cfg = &alarm_cfg,
	};
	struct counter_ioctl_us_to_ticks us_to_ticks = {
		.us = TEST_PERIOD_US,
	};
	struct counter_ioctl_get_frequency get_freq;
	uint32_t period_ticks;
	uint32_t counter_freq;
	uint32_t delta_cycles;
	uint32_t counter_jitter;
	int counter_fd;
	int loopback_out_fd;
	int loopback_in_fd;
	int ret;
	int err;

	loopback_out_fd = open("/dev/loopback_out", 0);
	if (loopback_out_fd < 0) {
		printk("open loopback_out failed: %d\n", errno);
		return 0;
	}

	loopback_in_fd = open("/dev/loopback_in", 0);
	if (loopback_in_fd < 0) {
		printk("open loopback_in failed: %d\n", errno);
		(void)close(loopback_out_fd);
		return 0;
	}

	counter_fd = open("/dev/counter0", 0);
	if (counter_fd < 0) {
		printk("open counter failed: %d\n", errno);
		(void)close(loopback_in_fd);
		(void)close(loopback_out_fd);
		return 0;
	}

	ret = ioctl(loopback_out_fd, GPIO_IOCTL_CONFIGURE, &out_cfg);
	if (ret < 0) {
		printk("Failed to configure output: %d\n", ret);
		goto out;
	}

	ret = ioctl(loopback_in_fd, GPIO_IOCTL_CONFIGURE, &in_cfg);
	if (ret < 0) {
		printk("Failed to configure input: %d\n", ret);
		goto out;
	}

	err = ioctl(counter_fd, COUNTER_IOCTL_US_TO_TICKS, &us_to_ticks);
	if (err < 0) {
		printk("counter_us_to_ticks failed: %d\n", err);
		goto out;
	}

	period_ticks = *us_to_ticks.ticks;
	if (period_ticks == 0U) {
		period_ticks = 1U;
	}

	k_sem_init(&sample_sem, 0, 1);

	err = ioctl(counter_fd, COUNTER_IOCTL_START);
	if (err < 0 && err != -EALREADY) {
		printk("counter_start failed: %d\n", err);
		goto out;
	}

	err = ioctl(counter_fd, COUNTER_IOCTL_GET_FREQUENCY, &get_freq);
	if (err < 0) {
		printk("counter_get_frequency failed: %d\n", err);
		goto out;
	}

	counter_freq = *get_freq.frequency;

	printk("PB1 one-shot loopback + timer ISR latency\n");
	printk("counter freq: %u Hz\n", counter_freq);
	printk("cpu cycles per sec: %u\n", sys_clock_hw_cycles_per_sec());
	printk("test period: %u us, counter ticks: %u\n", TEST_PERIOD_US, period_ticks);

	ret = ioctl(loopback_out_fd, GPIO_IOCTL_SET, &set_high);
	if (ret < 0) {
		printk("Failed to drive output: %d\n", ret);
		goto out;
	}

	k_msleep(1);

	ret = ioctl(loopback_in_fd, GPIO_IOCTL_GET);
	if (ret < 0) {
		printk("Failed to sample input: %d\n", ret);
		goto out;
	}

	printk("loopback drive=%d sample=%d %s\n",
	       set_high.value, ret, (ret == set_high.value) ? "PASS" : "FAIL");

	err = ioctl(counter_fd, COUNTER_IOCTL_GET_VALUE, &get_value);
	if (err < 0) {
		printk("counter_get_value failed: %d\n", err);
		goto out;
	}

	expected_counter_tick = measured_counter_tick + period_ticks;
	expected_cycle = k_cycle_get_32() + k_us_to_cyc_ceil32(TEST_PERIOD_US);
	sample_ready = 0U;
	alarm_cfg.ticks = expected_counter_tick;

	err = ioctl(counter_fd, COUNTER_IOCTL_SET_ALARM, &set_alarm);
	if (err < 0) {
		printk("counter_set_channel_alarm failed: %d\n", err);
		goto out;
	}

	k_sem_take(&sample_sem, K_FOREVER);
	if (!sample_ready) {
		printk("sample not ready\n");
		goto out;
	}

	delta_cycles = measured_cycle - expected_cycle;
	counter_jitter = measured_counter_tick - expected_counter_tick;

	printk("irq latency=%u cyc (%u ns) counter_jitter=%u\n",
	       delta_cycles,
	       cycles_to_ns_u32(delta_cycles),
	       counter_jitter);

out:
	(void)close(counter_fd);
	(void)close(loopback_in_fd);
	(void)close(loopback_out_fd);

	return 0;
}
