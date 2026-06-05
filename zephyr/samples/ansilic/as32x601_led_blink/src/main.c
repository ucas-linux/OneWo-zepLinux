#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

/*
 * On-board user LEDs (active low, anode tied to VCC_IC via resistor):
 *   led0 -> LED4 -> PG15
 *   led1 -> LED5 -> PG14
 *   led2 -> LED6 -> PG10
 */
#define LED0_NODE DT_ALIAS(led0)
#define LED1_NODE DT_ALIAS(led1)
#define LED2_NODE DT_ALIAS(led2)

#if !DT_NODE_HAS_STATUS(LED0_NODE, okay)
#error "Unsupported board: led0 alias is not defined"
#endif
#if !DT_NODE_HAS_STATUS(LED1_NODE, okay)
#error "Unsupported board: led1 alias is not defined"
#endif
#if !DT_NODE_HAS_STATUS(LED2_NODE, okay)
#error "Unsupported board: led2 alias is not defined"
#endif

static const struct gpio_dt_spec leds[] = {
	GPIO_DT_SPEC_GET(LED0_NODE, gpios),
	GPIO_DT_SPEC_GET(LED1_NODE, gpios),
	GPIO_DT_SPEC_GET(LED2_NODE, gpios),
};

int main(void)
{
	bool on = false;

	for (size_t i = 0; i < ARRAY_SIZE(leds); i++) {
		if (!gpio_is_ready_dt(&leds[i])) {
			return -ENODEV;
		}

		if (gpio_pin_configure_dt(&leds[i], GPIO_OUTPUT_INACTIVE) < 0) {
			return -EIO;
		}
	}

	while (1) {
		for (size_t i = 0; i < ARRAY_SIZE(leds); i++) {
			if (gpio_pin_set_dt(&leds[i], on ? 1 : 0) < 0) {
				return -EIO;
			}
		}

		printk("LED state: %s\n", on ? "ON" : "OFF");

		on = !on;
		k_msleep(1000);
	}
}
