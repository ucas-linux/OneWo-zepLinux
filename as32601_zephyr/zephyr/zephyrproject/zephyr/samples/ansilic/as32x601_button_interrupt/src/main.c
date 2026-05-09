#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

#include <zephyr/arch/riscv/csr.h>

#include <as32x601_gpio.h>
#include <as32x601_plic.h>

#define BTN0_NODE DT_ALIAS(sw0)
#define BTN1_NODE DT_ALIAS(sw1)
#define BTN2_NODE DT_ALIAS(sw2)

struct button_ctx {
	const struct gpio_dt_spec spec;
	const char *name;
	uint32_t count;
};

static struct button_ctx buttons[] = {
	{
		.spec = GPIO_DT_SPEC_GET(BTN0_NODE, gpios),
		.name = "PB3",
	},
	{
		.spec = GPIO_DT_SPEC_GET(BTN1_NODE, gpios),
		.name = "PB4",
	},
	{
		.spec = GPIO_DT_SPEC_GET(BTN2_NODE, gpios),
		.name = "PE10",
	},
};

static struct gpio_callback gpiob_callback;
static struct gpio_callback gpioe_callback;

static void button_isr(const struct device *port, struct gpio_callback *cb, uint32_t pins)
{
	ARG_UNUSED(cb);

	if (port == buttons[0].spec.port) {
		if ((pins & BIT(buttons[0].spec.pin)) != 0U) {
			buttons[0].count++;
		}
		if ((pins & BIT(buttons[1].spec.pin)) != 0U) {
			buttons[1].count++;
		}
	}

	if (port == buttons[2].spec.port) {
		if ((pins & BIT(buttons[2].spec.pin)) != 0U) {
			buttons[2].count++;
		}
	}

}

int main(void)
{
	int ret;

	printk("AS32X601 button interrupt test start\n");

	for (size_t i = 0; i < ARRAY_SIZE(buttons); i++) {
		if (!device_is_ready(buttons[i].spec.port)) {
			printk("button %s port not ready\n", buttons[i].name);
			return -ENODEV;
		}

		ret = gpio_pin_configure_dt(&buttons[i].spec, GPIO_INPUT | GPIO_PULL_UP);
		if (ret < 0) {
			printk("button %s configure failed: %d\n", buttons[i].name, ret);
			return ret;
		}

		ret = gpio_pin_interrupt_configure(buttons[i].spec.port,
					 buttons[i].spec.pin,
					 GPIO_INT_EDGE_FALLING);
		if (ret < 0) {
			printk("button %s irq config failed: %d\n", buttons[i].name, ret);
			return ret;
		}

		printk("button %s ready on pin %u\n",
		       buttons[i].name,
		       buttons[i].spec.pin);
	}

	gpiob_callback.node.next = NULL;
	gpiob_callback.handler = NULL;
	gpiob_callback.pin_mask = 0U;
	gpio_init_callback(&gpiob_callback, button_isr,
			       BIT(buttons[0].spec.pin) | BIT(buttons[1].spec.pin));
	ret = gpio_add_callback(buttons[0].spec.port, &gpiob_callback);
	if (ret < 0) {
		printk("GPIOB callback add failed: %d\n", ret);
		return ret;
	}

	gpioe_callback.node.next = NULL;
	gpioe_callback.handler = NULL;
	gpioe_callback.pin_mask = 0U;
	gpio_init_callback(&gpioe_callback, button_isr, BIT(buttons[2].spec.pin));
	ret = gpio_add_callback(buttons[2].spec.port, &gpioe_callback);
	if (ret < 0) {
		printk("GPIOE callback add failed: %d\n", ret);
		return ret;
	}

	while (1) {
		int v0 = gpio_pin_get_dt(&buttons[0].spec);
		int v1 = gpio_pin_get_dt(&buttons[1].spec);
		int v2 = gpio_pin_get_dt(&buttons[2].spec);

		printk("lvl PB3=%d PB4=%d PE10=%d cnt PB3=%u PB4=%u PE10=%u\n",
		       v0, v1, v2,
		       buttons[0].count,
		       buttons[1].count,
		       buttons[2].count);
		k_msleep(1000);
	}

	return 0;
}
