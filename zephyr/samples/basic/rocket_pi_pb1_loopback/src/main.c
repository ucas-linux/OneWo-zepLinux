#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#define LOOPBACK_OUT_NODE DT_ALIAS(loopback_out)
#define LOOPBACK_IN_NODE  DT_ALIAS(loopback_in)

#if !DT_NODE_HAS_STATUS(LOOPBACK_OUT_NODE, okay)
#error "Unsupported board: loopback_out devicetree alias is not defined"
#endif

#if !DT_NODE_HAS_STATUS(LOOPBACK_IN_NODE, okay)
#error "Unsupported board: loopback_in devicetree alias is not defined"
#endif

static const struct gpio_dt_spec loopback_out = GPIO_DT_SPEC_GET(LOOPBACK_OUT_NODE, gpios);
static const struct gpio_dt_spec loopback_in = GPIO_DT_SPEC_GET(LOOPBACK_IN_NODE, gpios);

int main(void)
{
	int ret;
	int level = 1;

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

	printk("PB1 loopback test started\n");
	ret = gpio_pin_set_dt(&loopback_out, level);
	if (ret < 0) {
		printk("Failed to drive output: %d\n", ret);
		return 0;
	}

	k_msleep(1);

	ret = gpio_pin_get_dt(&loopback_in);
	if (ret < 0) {
		printk("Failed to sample input: %d\n", ret);
		return 0;
	}

	printk("drive=%d sample=%d %s\n", level, ret, (ret == level) ? "PASS" : "FAIL");

	return 0;
}
