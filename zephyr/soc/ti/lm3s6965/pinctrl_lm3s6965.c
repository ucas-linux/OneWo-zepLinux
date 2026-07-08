/*
 * Copyright (c) 2025 UCAS
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/drivers/pinctrl.h>

/* Dummy pinctrl implementation for QEMU Cortex-M3 - no actual pin control needed */

int pinctrl_configure_pins(const pinctrl_soc_pin_t *pins, uint8_t pin_cnt,
			   uintptr_t reg)
{
	ARG_UNUSED(pins);
	ARG_UNUSED(pin_cnt);
	ARG_UNUSED(reg);

	/* Nothing to do on QEMU Cortex-M3 */
	return 0;
}
