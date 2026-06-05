/* SPDX-License-Identifier: Apache-2.0 */

#include <zephyr/drivers/pinctrl.h>
#include <zephyr/sys/util.h>
#include <zephyr/dt-bindings/pinctrl/ansilic-as32x601-pinctrl.h>

#include <as32x601_gpio.h>
#include <as32x601_smu.h>

static GPIO_TypeDef *as32x601_port_ptr(uint32_t port)
{
	switch (port) {
	case AS32X601_PORT_A:
		return GPIOA;
	case AS32X601_PORT_B:
		return GPIOB;
	case AS32X601_PORT_C:
		return GPIOC;
	case AS32X601_PORT_D:
		return GPIOD;
	case AS32X601_PORT_E:
		return GPIOE;
	case AS32X601_PORT_F:
		return GPIOF;
	case AS32X601_PORT_G:
		return GPIOG;
	default:
		return NULL;
	}
}

static int as32x601_port_clock_enable(uint32_t port)
{
	switch (port) {
	case AS32X601_PORT_A:
		GPIOA_CLK_ENABLE();
		return 0;
	case AS32X601_PORT_B:
		GPIOB_CLK_ENABLE();
		return 0;
	case AS32X601_PORT_C:
		GPIOC_CLK_ENABLE();
		return 0;
	case AS32X601_PORT_D:
		GPIOD_CLK_ENABLE();
		return 0;
	case AS32X601_PORT_E:
		GPIOE_CLK_ENABLE();
		return 0;
	case AS32X601_PORT_F:
		GPIOF_CLK_ENABLE();
		return 0;
	case AS32X601_PORT_G:
		GPIOG_CLK_ENABLE();
		return 0;
	default:
		return -EINVAL;
	}
}

static int as32x601_configure_pin(pinctrl_soc_pin_t pinmux)
{
	uint32_t port = (pinmux >> AS32X601_PIN_PORT_POS) & AS32X601_PIN_PORT_MASK;
	uint32_t pin = (pinmux >> AS32X601_PIN_NUM_POS) & AS32X601_PIN_NUM_MASK;
	uint32_t af = (pinmux >> AS32X601_PIN_AF_POS) & AS32X601_PIN_AF_MASK;
	uint32_t drive = (pinmux >> AS32X601_PIN_DRIVE_POS) & AS32X601_PIN_DRIVE_MASK;
	GPIO_TypeDef *gpio = as32x601_port_ptr(port);
	GPIO_InitTypeDef init;
	int ret;

	if (gpio == NULL) {
		return -EINVAL;
	}

	ret = as32x601_port_clock_enable(port);
	if (ret < 0) {
		return ret;
	}

	if (pinmux & BIT(AS32X601_PIN_ANALOG_POS)) {
		GPIO_StructInit(&init);
		init.GPIO_Pin   = BIT(pin);
		init.GPIO_Mode  = GPIO_Mode_AN;
		init.GPIO_IType = GPIO_IN_FLOATING;
		GPIO_Init(gpio, &init);
		return 0;
	}

	GPIO_StructInit(&init);
	init.GPIO_Pin = BIT(pin);
	init.GPIO_Mode = (pinmux & BIT(AS32X601_PIN_INPUT_ENABLE_POS)) ? GPIO_Mode_IN : GPIO_Mode_OUT;
	init.GPIO_IType = (pinmux & BIT(AS32X601_PIN_PULL_UP_POS)) ? GPIO_IPU :
		((pinmux & BIT(AS32X601_PIN_PULL_DOWN_POS)) ? GPIO_IPD : GPIO_IN_FLOATING);
	init.GPIO_OType = (pinmux & BIT(AS32X601_PIN_OPEN_DRAIN_POS)) ? GPIO_Out_OD : GPIO_Out_PP;
	init.GPIO_OStrength = drive;
	GPIO_Init(gpio, &init);
	GPIO_PinAFConfig(gpio, pin, af);

	if ((pinmux & BIT(AS32X601_PIN_OUTPUT_ENABLE_POS)) && (pinmux & BIT(AS32X601_PIN_OUTPUT_HIGH_POS))) {
		GPIO_SetBits(gpio, BIT(pin));
	} else if ((pinmux & BIT(AS32X601_PIN_OUTPUT_ENABLE_POS)) &&
		   (pinmux & BIT(AS32X601_PIN_OUTPUT_LOW_POS))) {
		GPIO_ClearBits(gpio, BIT(pin));
	}

	return 0;
}

int pinctrl_configure_pins(const pinctrl_soc_pin_t *pins, uint8_t pin_cnt, uintptr_t reg)
{
	for (uint8_t i = 0U; i < pin_cnt; i++) {
		int ret = as32x601_configure_pin(pins[i]);

		if (ret < 0) {
			return ret;
		}
	}

	ARG_UNUSED(pin_cnt);
	ARG_UNUSED(reg);

	return 0;
}
