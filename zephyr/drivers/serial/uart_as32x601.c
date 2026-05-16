/* SPDX-License-Identifier: Apache-2.0 */

#define DT_DRV_COMPAT ansilic_as32x601_uart

#include <errno.h>

#include <zephyr/device.h>
#include <zephyr/drivers/pinctrl.h>
#include <zephyr/drivers/uart.h>

#include <as32x601_smu.h>
#include <as32x601_gpio.h>
#include <as32x601_usart.h>

struct uart_as32x601_config {
	USART_TypeDef *uart;
	uint32_t baudrate;
	const struct pinctrl_dev_config *pcfg;
};

struct uart_as32x601_data {
	struct uart_config cfg;
};

static int uart_as32x601_apply_config(const struct uart_as32x601_config *cfg,
					      const struct uart_config *uart_cfg)
{
	USART_InitTypeDef init;

	if ((uart_cfg->data_bits != UART_CFG_DATA_BITS_8) ||
	    (uart_cfg->stop_bits != UART_CFG_STOP_BITS_1) ||
	    (uart_cfg->flow_ctrl != UART_CFG_FLOW_CTRL_NONE)) {
		return -ENOTSUP;
	}

	USART_DeInit(cfg->uart);
	USART_StructInit(&init);
	init.USART_BaudRate = uart_cfg->baudrate;
	init.USART_StopBits = USART_StopBits_1;
	init.USART_WordLength = USART_WordLength_8b;
	init.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	init.USART_HardwareFlowControl = USART_HWControl_No;
	init.USART_Parity = (uart_cfg->parity == UART_CFG_PARITY_ODD) ? USART_Parity_Odd :
		((uart_cfg->parity == UART_CFG_PARITY_EVEN) ? USART_Parity_Even : USART_Parity_No);
	USART_Init(cfg->uart, &init);
	USART_Cmd(cfg->uart, ENABLE);

	return 0;
}

static int uart_as32x601_poll_in(const struct device *dev, unsigned char *c)
{
	const struct uart_as32x601_config *cfg = dev->config;

	if ((cfg->uart->ISR & USART_FLAG_RXNE) == 0U) {
		return -1;
	}

	*c = (unsigned char)(cfg->uart->RDR & 0xFFU);
	return 0;
}

static void uart_as32x601_poll_out(const struct device *dev, unsigned char c)
{
	const struct uart_as32x601_config *cfg = dev->config;

	while ((cfg->uart->ISR & USART_FLAG_TXE) == 0U) {
	}

	cfg->uart->ICR = USART_CLEAR_TCF;
	cfg->uart->TDR = c;

	while ((cfg->uart->ISR & USART_FLAG_TC) == 0U) {
	}
}

static int uart_as32x601_err_check(const struct device *dev)
{
	const struct uart_as32x601_config *cfg = dev->config;
	int err = 0;
	uint32_t isr = cfg->uart->ISR;

	if (isr & USART_ISR_ORE) {
		err |= UART_ERROR_OVERRUN;
	}
	if (isr & USART_ISR_PE) {
		err |= UART_ERROR_PARITY;
	}
	if (isr & USART_ISR_FE) {
		err |= UART_ERROR_FRAMING;
	}

	return err;
}

#ifdef CONFIG_UART_USE_RUNTIME_CONFIGURE
static int uart_as32x601_configure(const struct device *dev,
					   const struct uart_config *cfg)
{
	const struct uart_as32x601_config *config = dev->config;
	struct uart_as32x601_data *data = dev->data;
	int ret = uart_as32x601_apply_config(config, cfg);

	if (ret == 0) {
		data->cfg = *cfg;
	}

	return ret;
}

static int uart_as32x601_config_get(const struct device *dev,
					   struct uart_config *cfg)
{
	struct uart_as32x601_data *data = dev->data;

	*cfg = data->cfg;
	return 0;
}
#endif

static int uart_as32x601_init(const struct device *dev)
{
	const struct uart_as32x601_config *cfg = dev->config;
	struct uart_as32x601_data *data = dev->data;
	int ret;

	if (cfg->uart == USART0) {
		USART0_CLK_ENABLE();
	} else if (cfg->uart == USART5) {
		USART5_CLK_ENABLE();
	} else {
		return -ENOTSUP;
	}

	ret = pinctrl_apply_state(cfg->pcfg, PINCTRL_STATE_DEFAULT);
	if (ret < 0) {
		return ret;
	}

	data->cfg = (struct uart_config) {
		.baudrate = cfg->baudrate,
		.parity = UART_CFG_PARITY_NONE,
		.stop_bits = UART_CFG_STOP_BITS_1,
		.data_bits = UART_CFG_DATA_BITS_8,
		.flow_ctrl = UART_CFG_FLOW_CTRL_NONE,
	};

	ret = uart_as32x601_apply_config(cfg, &data->cfg);
	if (ret < 0) {
		return ret;
	}

	return 0;
}

static DEVICE_API(uart, uart_as32x601_api) = {
	.poll_in = uart_as32x601_poll_in,
	.poll_out = uart_as32x601_poll_out,
	.err_check = uart_as32x601_err_check,
#ifdef CONFIG_UART_USE_RUNTIME_CONFIGURE
	.configure = uart_as32x601_configure,
	.config_get = uart_as32x601_config_get,
#endif
};

#define UART_AS32X601_INIT(inst) \
	PINCTRL_DT_INST_DEFINE(inst); \
	static const struct uart_as32x601_config uart_as32x601_config_##inst = { \
		.uart = (USART_TypeDef *)DT_INST_REG_ADDR(inst), \
		.baudrate = DT_INST_PROP(inst, current_speed), \
		.pcfg = PINCTRL_DT_INST_DEV_CONFIG_GET(inst), \
	}; \
	static struct uart_as32x601_data uart_as32x601_data_##inst; \
	DEVICE_DT_INST_DEFINE(inst, uart_as32x601_init, NULL, \
		&uart_as32x601_data_##inst, &uart_as32x601_config_##inst, \
		PRE_KERNEL_1, CONFIG_SERIAL_INIT_PRIORITY, &uart_as32x601_api);

DT_INST_FOREACH_STATUS_OKAY(UART_AS32X601_INIT)
