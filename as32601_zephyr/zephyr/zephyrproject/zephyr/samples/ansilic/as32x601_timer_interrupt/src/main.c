/* SPDX-License-Identifier: Apache-2.0 */
/* Copyright (c) 2024 Ansilic Technology Co., Ltd */

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/counter.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#define LED0_NODE DT_ALIAS(led0)
#define TIM0_NODE DT_NODELABEL(tim0)

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static volatile uint32_t tick_count;
static volatile bool toggle_flag;

static void timer_top_cb(const struct device *dev, void *user_data)
{
	ARG_UNUSED(dev);
	ARG_UNUSED(user_data);

	tick_count++;
	if (tick_count >= 1000) {
		tick_count = 0;
		toggle_flag = true;
	}
}

int main(void)
{
	const struct device *tim0 = DEVICE_DT_GET(TIM0_NODE);

	printk("AS32X601 Timer interrupt sample start\n");

	if (!device_is_ready(tim0)) {
		printk("tim0 device not ready\n");
		return -ENODEV;
	}

	if (!gpio_is_ready_dt(&led)) {
		printk("led0 device not ready\n");
		return -ENODEV;
	}

	if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE) < 0) {
		printk("led0 configure failed\n");
		return -EIO;
	}

	struct counter_top_cfg top_cfg = {
		.ticks     = 44999,
		.callback  = timer_top_cb,
		.user_data = NULL,
		.flags     = 0,
	};

	int ret = counter_set_top_value(tim0, &top_cfg);

	if (ret != 0) {
		printk("counter_set_top_value failed: %d\n", ret);
		return ret;
	}

	ret = counter_start(tim0);
	if (ret != 0) {
		printk("counter_start failed: %d\n", ret);
		return ret;
	}

	printk("Timer started, ARR=44999, ~1ms period, LED toggles every 1s\n");

	while (1) {
		k_cpu_idle();
		if (toggle_flag) {
			toggle_flag = false;
			printk("TickCount = 1000\n");
			gpio_pin_toggle_dt(&led);
		}
	}

	return 0;
}
