/* SPDX-License-Identifier: Apache-2.0 */
#ifndef ZEPHYR_SOC_ANSILIC_AS32X601_PINCTRL_SOC_H_
#define ZEPHYR_SOC_ANSILIC_AS32X601_PINCTRL_SOC_H_

#include <stdint.h>
#include <zephyr/devicetree.h>
#include <zephyr/dt-bindings/pinctrl/ansilic-as32x601-pinctrl.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @cond INTERNAL_HIDDEN */

typedef uint32_t pinctrl_soc_pin_t;

#define Z_PINCTRL_STATE_PINCFG_INIT(node_id) \
	((DT_PROP_OR(node_id, bias_pull_up, 0) << AS32X601_PIN_PULL_UP_POS) | \
	 (DT_PROP_OR(node_id, bias_pull_down, 0) << AS32X601_PIN_PULL_DOWN_POS) | \
	 (DT_PROP_OR(node_id, drive_open_drain, 0) << AS32X601_PIN_OPEN_DRAIN_POS) | \
	 (DT_PROP_OR(node_id, drive_push_pull, 0) << AS32X601_PIN_PUSH_PULL_POS) | \
	 (DT_PROP_OR(node_id, output_high, 0) << AS32X601_PIN_OUTPUT_HIGH_POS) | \
	 (DT_PROP_OR(node_id, output_low, 0) << AS32X601_PIN_OUTPUT_LOW_POS) | \
	 (DT_PROP_OR(node_id, input_enable, 0) << AS32X601_PIN_INPUT_ENABLE_POS) | \
	 (DT_PROP_OR(node_id, output_enable, 0) << AS32X601_PIN_OUTPUT_ENABLE_POS) | \
	 (DT_PROP_OR(node_id, slew_rate, AS32X601_GPIO_DRIVE_9MA) << AS32X601_PIN_DRIVE_POS))

#define Z_PINCTRL_STATE_PIN_INIT(group_node_id, prop, idx) \
	(DT_PROP_BY_IDX(group_node_id, prop, idx) | Z_PINCTRL_STATE_PINCFG_INIT(group_node_id)),

#define Z_PINCTRL_STATE_PINS_INIT(node_id, prop) \
	{DT_FOREACH_CHILD_VARGS(DT_PHANDLE(node_id, prop), \
				DT_FOREACH_PROP_ELEM, pinmux, \
				Z_PINCTRL_STATE_PIN_INIT)}

/** @endcond */

#ifdef __cplusplus
}
#endif

#endif
