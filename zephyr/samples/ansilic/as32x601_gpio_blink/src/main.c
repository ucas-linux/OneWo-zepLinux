#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#define LED0_NODE DT_ALIAS(led0)

#if !DT_NODE_HAS_STATUS(LED0_NODE, okay)
#error "Unsupported board: led0 alias is not defined"
#endif

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

int main(void)
{
	bool on = false;

	printk("AS32X601 GPIO blink test start\n");

	if (!gpio_is_ready_dt(&led)) {
		printk("led0 device not ready\n");
		return -ENODEV;
	}

	if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE) < 0) {
		printk("led0 configure failed\n");
		return -EIO;
	}

	printk("led0 mapped to PG15, active_low=%d\n", (led.dt_flags & GPIO_ACTIVE_LOW) ? 1 : 0);

	while (1) {
		if (gpio_pin_set_dt(&led, on ? 1 : 0) < 0) {
			printk("led0 set failed\n");
			return -EIO;
		}

		printk("PG15 set=%d\n", on ? 1 : 0);

		on = !on;
		k_msleep(1000);
	}
}
