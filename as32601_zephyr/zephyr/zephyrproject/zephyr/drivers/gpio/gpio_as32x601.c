/* SPDX-License-Identifier: Apache-2.0 */
#define DT_DRV_COMPAT ansilic_as32x601_gpio

#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/gpio/gpio_utils.h>
#include <zephyr/drivers/interrupt_controller/riscv_plic.h>
#include <zephyr/irq_multilevel.h>
#include <zephyr/kernel.h>
#include <zephyr/irq.h>
#include <zephyr/sys/printk.h>
#include <zephyr/spinlock.h>

#include <as32x601_gpio.h>
#include <as32x601_plic.h>
#include <as32x601_smu.h>
#include <soc.h>

#define AS32X601_ZEPHYR_L2_IRQ(src) ((RISCV_IRQ_MEXT << CONFIG_1ST_LEVEL_INTERRUPT_BITS) | (src))

struct as32x601_gpio_config {
	struct gpio_driver_config common;
	GPIO_TypeDef *port;
	uint32_t plic_irq;
	uint32_t port_addr;
};

struct as32x601_gpio_data {
	struct gpio_driver_data common;
	struct k_spinlock lock;
	gpio_port_pins_t output_state;
	gpio_port_pins_t invert;
	uint32_t irq_hits;
	uint32_t irq_last_pending;
	sys_slist_t callbacks;
};


static GPIO_TypeDef *as32x601_gpio_port_from_addr(uint32_t port_addr)
{

	switch (port_addr) {
	case AS32X601_GPIOA_BASE:
		return GPIOA;
	case AS32X601_GPIOB_BASE:
		return GPIOB;
	case AS32X601_GPIOC_BASE:
		return GPIOC;
	case AS32X601_GPIOD_BASE:
		return GPIOD;
	case AS32X601_GPIOE_BASE:
		return GPIOE;
	case AS32X601_GPIOF_BASE:
		return GPIOF;
	case AS32X601_GPIOG_BASE:
		return GPIOG;
	case AS32X601_GPIOH_BASE:
		return GPIOH;
	default:
		return NULL;
	}
}

static int as32x601_gpio_enable_clock(uint32_t port_addr)
{
	switch (port_addr) {
	case 0x41004000:
		GPIOA_CLK_ENABLE();
		return 0;
	case 0x41005000:
		GPIOB_CLK_ENABLE();
		return 0;
	case 0x41006000:
		GPIOC_CLK_ENABLE();
		return 0;
	case 0x42003000:
		GPIOD_CLK_ENABLE();
		return 0;
	case 0x42004000:
		GPIOE_CLK_ENABLE();
		return 0;
	case 0x42005000:
		GPIOF_CLK_ENABLE();
		return 0;
	case 0x42006000:
		GPIOG_CLK_ENABLE();
		return 0;
	case 0x30102000:
		return 0;
	default:
		return -EINVAL;
	}
}

static int as32x601_gpio_pin_configure(const struct device *dev, gpio_pin_t pin, gpio_flags_t flags)
{
	const struct as32x601_gpio_config *cfg = dev->config;
	GPIO_TypeDef *port = as32x601_gpio_port_from_addr(cfg->port_addr);
	struct as32x601_gpio_data *data = dev->data;
	GPIO_InitTypeDef init;
	k_spinlock_key_t key;
	uint32_t pin_mask;

	if (port == NULL) {
		return -ENODEV;
	}

	if ((BIT(pin) & cfg->common.port_pin_mask) == 0U) {
		return -EINVAL;
	}

	if ((flags & GPIO_DISCONNECTED) != 0U) {
		return -ENOTSUP;
	}

	if ((flags & GPIO_PULL_UP) && (flags & GPIO_PULL_DOWN)) {
		return -ENOTSUP;
	}

	pin_mask = BIT(pin);
	GPIO_StructInit(&init);
	init.GPIO_Pin = pin_mask;
	init.GPIO_OStrength = GPIO_OStrength_9mA;

	if ((flags & GPIO_OPEN_DRAIN) != 0U) {
		init.GPIO_OType = GPIO_Out_OD;
	} else {
		init.GPIO_OType = GPIO_Out_PP;
	}

	if ((flags & GPIO_PULL_UP) != 0U) {
		init.GPIO_IType = GPIO_IPU;
	} else if ((flags & GPIO_PULL_DOWN) != 0U) {
		init.GPIO_IType = GPIO_IPD;
	} else {
		init.GPIO_IType = GPIO_IN_FLOATING;
	}

	if ((flags & GPIO_ACTIVE_LOW) != 0U) {
		data->invert |= pin_mask;
	} else {
		data->invert &= ~pin_mask;
	}

	if ((flags & GPIO_OUTPUT) != 0U) {
		init.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_Init(port, &init);

		key = k_spin_lock(&data->lock);
		if ((flags & GPIO_OUTPUT_INIT_HIGH) != 0U) {
			GPIO_SetBits(port, pin_mask);
			data->output_state |= pin_mask;
		} else {
			GPIO_ClearBits(port, pin_mask);
			data->output_state &= ~pin_mask;
		}
		k_spin_unlock(&data->lock, key);
		return 0;
	}

	if ((flags & GPIO_INPUT) != 0U) {
		init.GPIO_Mode = GPIO_Mode_IN;
		GPIO_Init(port, &init);
		return 0;
	}

	return -ENOTSUP;
}

static int as32x601_gpio_port_get_raw(const struct device *dev, gpio_port_value_t *value)
{
	const struct as32x601_gpio_config *cfg = dev->config;
	GPIO_TypeDef *port = as32x601_gpio_port_from_addr(cfg->port_addr);

	if (port == NULL) {
		return -ENODEV;
	}

	*value = GPIO_ReadInputData(port);
	return 0;
}

static int as32x601_gpio_port_set_masked_raw(const struct device *dev, gpio_port_pins_t mask,
						     gpio_port_value_t value)
{
	const struct as32x601_gpio_config *cfg = dev->config;
	GPIO_TypeDef *port = as32x601_gpio_port_from_addr(cfg->port_addr);
	struct as32x601_gpio_data *data = dev->data;
	k_spinlock_key_t key = k_spin_lock(&data->lock);

	if (port == NULL) {
		k_spin_unlock(&data->lock, key);
		return -ENODEV;
	}

	data->output_state = (data->output_state & ~mask) | (value & mask);
	GPIO_Write(port, (uint16_t)data->output_state);
	k_spin_unlock(&data->lock, key);
	return 0;
}

static int as32x601_gpio_port_set_bits_raw(const struct device *dev, gpio_port_pins_t pins)
{
	const struct as32x601_gpio_config *cfg = dev->config;
	GPIO_TypeDef *port = as32x601_gpio_port_from_addr(cfg->port_addr);
	struct as32x601_gpio_data *data = dev->data;
	k_spinlock_key_t key = k_spin_lock(&data->lock);

	if (port == NULL) {
		k_spin_unlock(&data->lock, key);
		return -ENODEV;
	}

	data->output_state |= pins;
	GPIO_SetBits(port, pins);
	k_spin_unlock(&data->lock, key);
	return 0;
}

static int as32x601_gpio_port_clear_bits_raw(const struct device *dev, gpio_port_pins_t pins)
{
	const struct as32x601_gpio_config *cfg = dev->config;
	GPIO_TypeDef *port = as32x601_gpio_port_from_addr(cfg->port_addr);
	struct as32x601_gpio_data *data = dev->data;
	k_spinlock_key_t key = k_spin_lock(&data->lock);

	if (port == NULL) {
		k_spin_unlock(&data->lock, key);
		return -ENODEV;
	}

	data->output_state &= ~pins;
	GPIO_ClearBits(port, pins);
	k_spin_unlock(&data->lock, key);
	return 0;
}

static int as32x601_gpio_port_toggle_bits(const struct device *dev, gpio_port_pins_t pins)
{
	const struct as32x601_gpio_config *cfg = dev->config;
	GPIO_TypeDef *port = as32x601_gpio_port_from_addr(cfg->port_addr);
	struct as32x601_gpio_data *data = dev->data;
	k_spinlock_key_t key = k_spin_lock(&data->lock);

	if (port == NULL) {
		k_spin_unlock(&data->lock, key);
		return -ENODEV;
	}

	data->output_state ^= pins;
	GPIO_ToggleBits(port, pins);
	k_spin_unlock(&data->lock, key);
	return 0;
}

static int as32x601_gpio_pin_interrupt_configure(const struct device *dev, gpio_pin_t pin,
						 enum gpio_int_mode mode,
						 enum gpio_int_trig trig)
{
	const struct as32x601_gpio_config *cfg = dev->config;
	GPIO_TypeDef *port = as32x601_gpio_port_from_addr(cfg->port_addr);
	struct as32x601_gpio_data *data = dev->data;
	GPIOITType_TypeDef it_type;
	PLIC_InitTypeDef plic_init;
	bool active_low;

	if (port == NULL) {
		return -ENODEV;
	}

	if ((BIT(pin) & cfg->common.port_pin_mask) == 0U) {
		return -EINVAL;
	}

	if (mode == GPIO_INT_MODE_DISABLED) {
		GPIO_ITConfig(port, BIT(pin), GPIO_ITType_CLOSE, DISABLE);
		GPIO_ClearITPendingBit(port, BIT(pin));
		return 0;
	}

	if (mode != GPIO_INT_MODE_EDGE) {
		return -ENOTSUP;
	}

	active_low = (data->invert & BIT(pin)) != 0U;

	if ((mode & GPIO_INT_LEVELS_LOGICAL) != 0U) {
		switch (trig) {
		case GPIO_INT_TRIG_LOW:
			trig = active_low ? GPIO_INT_TRIG_HIGH : GPIO_INT_TRIG_LOW;
			break;
		case GPIO_INT_TRIG_HIGH:
			trig = active_low ? GPIO_INT_TRIG_LOW : GPIO_INT_TRIG_HIGH;
			break;
		case GPIO_INT_TRIG_BOTH:
			break;
		default:
			return -ENOTSUP;
		}
	}

	switch (trig) {
	case GPIO_INT_TRIG_LOW:
		it_type = GPIO_ITType_EDGEDOWN;
		break;
	case GPIO_INT_TRIG_HIGH:
		it_type = GPIO_ITType_EDGEUP;
		break;
	case GPIO_INT_TRIG_BOTH:
		return -ENOTSUP;
	default:
		return -ENOTSUP;
	}

	GPIO_ClearITPendingBit(port, BIT(pin));
	GPIO_ITConfig(port, BIT(pin), it_type, ENABLE);
	PLIC_StructInit(&plic_init);
	plic_init.PLIC_IRQChannel = (PLICIRQn_TypeDef)cfg->plic_irq;
	if (cfg->plic_irq >= 32U) {
		plic_init.PLIC_IRQPriority = 2U;
	} else {
		plic_init.PLIC_IRQPriority = 1U;
	}
	plic_init.PLIC_IRQChannelCmd = ENABLE;
	PLIC_Init(&plic_init);
	return 0;
}

static int as32x601_gpio_manage_callback(const struct device *dev,
					 struct gpio_callback *callback, bool set)
{
	struct as32x601_gpio_data *data = dev->data;

	if (set) {
		callback->node.next = NULL;
	}

	return gpio_manage_callback(&data->callbacks, callback, set);
}

static uint32_t as32x601_gpio_get_pending_int(const struct device *dev)
{
	const struct as32x601_gpio_config *cfg = dev->config;
	GPIO_TypeDef *port = as32x601_gpio_port_from_addr(cfg->port_addr);

	if (port == NULL) {
		return 0U;
	}

	return port->ISR;
}

static void as32x601_gpio_isr(const struct device *dev)
{
	const struct as32x601_gpio_config *cfg = dev->config;
	struct as32x601_gpio_data *data = dev->data;
	GPIO_TypeDef *port = as32x601_gpio_port_from_addr(cfg->port_addr);
	uint32_t pending = 0U;
	uint32_t valid_mask;
	uint32_t bit;

	if (port == NULL) {
		return;
	}

	valid_mask = cfg->common.port_pin_mask;
	for (bit = 0U; bit < 16U; bit++) {
		uint32_t pin = BIT(bit);

		if ((valid_mask & pin) == 0U) {
			continue;
		}

		if (GPIO_GetITStatus(port, pin) == SET) {
			pending |= pin;
		}
	}

	if (pending == 0U) {
		return;
	}

	GPIO_ClearITPendingBit(port, pending);
	data->irq_hits++;
	data->irq_last_pending = pending;
	gpio_fire_callbacks(&data->callbacks, dev, pending);
}

void GPIOA_IRQ_Handler(void)
{
	as32x601_gpio_isr(DEVICE_DT_GET(DT_NODELABEL(gpioa)));
}

void GPIOB_IRQ_Handler(void)
{
	as32x601_gpio_isr(DEVICE_DT_GET(DT_NODELABEL(gpiob)));
}

void GPIOE_IRQ_Handler(void)
{
	as32x601_gpio_isr(DEVICE_DT_GET(DT_NODELABEL(gpioe)));
}

void GPIOG_IRQ_Handler(void)
{
	as32x601_gpio_isr(DEVICE_DT_GET(DT_NODELABEL(gpiog)));
}

static int as32x601_gpio_init(const struct device *dev)
{
	const struct as32x601_gpio_config *cfg = dev->config;
	struct as32x601_gpio_data *data = dev->data;
	int ret = as32x601_gpio_enable_clock(cfg->port_addr);

	if (ret < 0) {
		return ret;
	}

	if (cfg->port == NULL) {
		return -ENODEV;
	}

	data->output_state = GPIO_ReadOutputData(cfg->port);
	sys_slist_init(&data->callbacks);

	return 0;
}

static DEVICE_API(gpio, as32x601_gpio_api) = {
	.pin_configure = as32x601_gpio_pin_configure,
	.port_get_raw = as32x601_gpio_port_get_raw,
	.port_set_masked_raw = as32x601_gpio_port_set_masked_raw,
	.port_set_bits_raw = as32x601_gpio_port_set_bits_raw,
	.port_clear_bits_raw = as32x601_gpio_port_clear_bits_raw,
	.port_toggle_bits = as32x601_gpio_port_toggle_bits,
	.pin_interrupt_configure = as32x601_gpio_pin_interrupt_configure,
	.manage_callback = as32x601_gpio_manage_callback,
	.get_pending_int = as32x601_gpio_get_pending_int,
};


#define AS32X601_GPIO_PORT_INIT(inst) \
	static const struct as32x601_gpio_config as32x601_gpio_cfg_##inst = { \
		.common = { \
			.port_pin_mask = GPIO_PORT_PIN_MASK_FROM_DT_INST(inst), \
		}, \
		.port = (GPIO_TypeDef *)DT_INST_REG_ADDR(inst), \
		.plic_irq = DT_INST_IRQ_BY_IDX(inst, 0, irq), \
		.port_addr = DT_INST_REG_ADDR(inst), \
	}; \
	static struct as32x601_gpio_data as32x601_gpio_data_##inst; \
	DEVICE_DT_INST_DEFINE(inst, as32x601_gpio_init, NULL, \
			      &as32x601_gpio_data_##inst, &as32x601_gpio_cfg_##inst, \
			      PRE_KERNEL_1, CONFIG_GPIO_INIT_PRIORITY, &as32x601_gpio_api);

DT_INST_FOREACH_STATUS_OKAY(AS32X601_GPIO_PORT_INIT)
