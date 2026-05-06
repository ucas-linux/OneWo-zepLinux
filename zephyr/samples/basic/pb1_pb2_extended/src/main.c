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

#define TOGGLE_COUNT 20
#define SETTLE_MS    1
#define BURST_DELAY_MS 20

static const struct gpio_dt_spec loopback_out = GPIO_DT_SPEC_GET(LOOPBACK_OUT_NODE, gpios);
static const struct gpio_dt_spec loopback_in = GPIO_DT_SPEC_GET(LOOPBACK_IN_NODE, gpios);

static int sample_and_check(int level, const char *tag)
{
	int ret;

	ret = gpio_pin_set_dt(&loopback_out, level);
	if (ret < 0) {
		printk("%s drive failed: %d\n", tag, ret);
		return ret;
	}

	k_msleep(SETTLE_MS);

	ret = gpio_pin_get_dt(&loopback_in);
	if (ret < 0) {
		printk("%s sample failed: %d\n", tag, ret);
		return ret;
	}

	printk("%s drive=%d sample=%d %s\n",
	       tag, level, ret, (ret == level) ? "PASS" : "FAIL");

	return (ret == level) ? 0 : -EIO;
}

int main(void)
{
	int ret;
	int failures = 0;
	int pass_count = 0;

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

	printk("PB1->PB2 extended test start\n");

	if (sample_and_check(0, "single-low") == 0) {
		pass_count++;
	} else {
		failures++;
	}

	if (sample_and_check(1, "single-high") == 0) {
		pass_count++;
	} else {
		failures++;
	}

	for (int i = 0; i < TOGGLE_COUNT; i++) {
		int level = i & 1;
		char tag[24];

		snprintk(tag, sizeof(tag), "toggle-%02d", i + 1);
		if (sample_and_check(level, tag) == 0) {
			pass_count++;
		} else {
			failures++;
		}
	}

	printk("burst-high-hold\n");
	ret = gpio_pin_set_dt(&loopback_out, 1);
	if (ret < 0) {
		printk("burst-high drive failed: %d\n", ret);
		failures++;
	} else {
		k_msleep(BURST_DELAY_MS);
		ret = gpio_pin_get_dt(&loopback_in);
		if (ret == 1) {
			printk("burst-high sample=1 PASS\n");
			pass_count++;
		} else {
			printk("burst-high sample=%d FAIL\n", ret);
			failures++;
		}
	}

	printk("burst-low-hold\n");
	ret = gpio_pin_set_dt(&loopback_out, 0);
	if (ret < 0) {
		printk("burst-low drive failed: %d\n", ret);
		failures++;
	} else {
		k_msleep(BURST_DELAY_MS);
		ret = gpio_pin_get_dt(&loopback_in);
		if (ret == 0) {
			printk("burst-low sample=0 PASS\n");
			pass_count++;
		} else {
			printk("burst-low sample=%d FAIL\n", ret);
			failures++;
		}
	}

	printk("PB1->PB2 extended done: pass=%d fail=%d\n", pass_count, failures);
	return 0;
}
