/* SPDX-License-Identifier: Apache-2.0 */

#define DT_DRV_COMPAT ansilic_as32x601_i2c

#include <errno.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <zephyr/drivers/pinctrl.h>
#include <zephyr/kernel.h>

#include <as32x601_smu.h>
#include <as32x601_i2c.h>

/* State codes */
#define I2C_STATE_START        0x01U
#define I2C_STATE_RSTART       0x02U
#define I2C_STATE_WADDR_ACK    0x03U
#define I2C_STATE_DATA_ACK     0x05U
#define I2C_STATE_RADDR_ACK    0x08U
#define I2C_STATE_RDATA_ACK    0x0AU
#define I2C_STATE_RDATA_NACK   0x0BU
#define I2C_STATE_STOP         0x1CU
#define I2C_STATE_IDLE         0x1FU

#define I2C_TIMEOUT_US         10000
#define I2C_IDLE_RECOVERY_RETRIES 1000

/* Match the vendor BSP 100 kHz timing values used by the AS32X601 EVB. */
#define I2C_100K_DIVH  0x00U
#define I2C_100K_DIVL  0x60U

struct i2c_as32x601_config {
	I2C_TypeDef *i2c;
	uint32_t clock_freq;
	const struct pinctrl_dev_config *pcfg;
};

struct i2c_as32x601_data {
	struct k_mutex lock;
};

static int wait_state(I2C_TypeDef *i2c, uint8_t expected)
{
	for (int i = 0; i < I2C_TIMEOUT_US; i++) {
		if (I2C_GetFlagstatus(i2c) == expected) {
			return 0;
		}
		k_busy_wait(1U);
	}

	return -EIO;
}

static int wait_state2(I2C_TypeDef *i2c, uint8_t a, uint8_t b)
{
	for (int i = 0; i < I2C_TIMEOUT_US; i++) {
		uint8_t s = I2C_GetFlagstatus(i2c);

		if (s == a || s == b) {
			return 0;
		}
		k_busy_wait(1U);
	}
	return -EIO;
}

static int wait_bus_idle(I2C_TypeDef *i2c)
{
	for (int i = 0; i < I2C_IDLE_RECOVERY_RETRIES; i++) {
		if (I2C_GetFlagstatus(i2c) == I2C_STATE_IDLE) {
			return 0;
		}

		I2C_StartClear(i2c);
		I2C_GenerateStop(i2c);
		I2C_ClearITPendingBit(i2c);
		k_busy_wait(500U);
	}

	return -EIO;
}

static int i2c_as32x601_transfer(const struct device *dev,
				  struct i2c_msg *msgs, uint8_t num_msgs,
				  uint16_t addr)
{
	const struct i2c_as32x601_config *cfg = dev->config;
	struct i2c_as32x601_data *data = dev->data;
	I2C_TypeDef *i2c = cfg->i2c;
	int ret = 0;

	k_mutex_lock(&data->lock, K_FOREVER);

	ret = wait_bus_idle(i2c);
	if (ret != 0) {
		goto out;
	}

	for (uint8_t m = 0U; m < num_msgs; m++) {
		struct i2c_msg *msg = &msgs[m];
		bool is_read = (msg->flags & I2C_MSG_READ) != 0U;

		/*
		 * Generate START. For the first message the bus is idle and the
		 * controller sends START automatically. For subsequent messages
		 * (repeated START) the controller is halted from the previous
		 * data-ACK state; ClearITPendingBit is needed to trigger RSTART.
		 */
		I2C_GenerateStart(i2c);
		if (m > 0U) {
			I2C_ClearITPendingBit(i2c);
		}
		ret = wait_state2(i2c, I2C_STATE_START, I2C_STATE_RSTART);
		if (ret != 0) {
			goto out;
		}

		/*
		 * Send address. StartClear clears the STA bit so the controller
		 * does not re-issue START. ClearITPendingBit advances the state
		 * machine and triggers address transmission on the bus.
		 */
		I2C_Send7bitAddress(i2c, (uint8_t)(addr & 0x7FU),
				    is_read ? I2C_READ : I2C_WRITE);
		I2C_StartClear(i2c);
		I2C_ClearITPendingBit(i2c);
		ret = wait_state(i2c, is_read ? I2C_STATE_RADDR_ACK : I2C_STATE_WADDR_ACK);
		if (ret != 0) {
			ret = -ENXIO;
			goto out;
		}

		if (is_read) {
			for (uint32_t i = 0U; i < msg->len; i++) {
				bool last = (i == msg->len - 1U);

				I2C_AcknowledgeConfig(i2c,
					last ? I2C_IICAA_NACK : I2C_IICAA_ACK);
				/* StartClear + ClearIT triggers clock for receiving the byte */
				I2C_StartClear(i2c);
				I2C_ClearITPendingBit(i2c);
				ret = wait_state(i2c,
					last ? I2C_STATE_RDATA_NACK : I2C_STATE_RDATA_ACK);
				if (ret != 0) {
					goto out;
				}
				msg->buf[i] = I2C_ReceiveData(i2c);
			}
		} else {
			for (uint32_t i = 0U; i < msg->len; i++) {
				I2C_SendData(i2c, msg->buf[i]);
				/* ClearIT triggers transmission of the byte */
				I2C_ClearITPendingBit(i2c);
				ret = wait_state(i2c, I2C_STATE_DATA_ACK);
				if (ret != 0) {
					goto out;
				}
			}
		}

		if (msg->flags & I2C_MSG_STOP) {
			I2C_StartClear(i2c);
			I2C_GenerateStop(i2c);
			I2C_ClearITPendingBit(i2c);
			ret = wait_state(i2c, I2C_STATE_STOP);
			if (ret != 0) {
				goto out;
			}
		}
	}

out:
	if (ret != 0) {
		I2C_StartClear(i2c);
		I2C_GenerateStop(i2c);
		I2C_ClearITPendingBit(i2c);
	}
	k_mutex_unlock(&data->lock);
	return ret;
}

static int i2c_as32x601_init(const struct device *dev)
{
	const struct i2c_as32x601_config *cfg = dev->config;
	struct i2c_as32x601_data *data = dev->data;
	I2C_InitTypeDef init;
	int ret;

	if ((uintptr_t)cfg->i2c == 0x50004000U) {
		IIC0_CLK_ENABLE();
	} else if ((uintptr_t)cfg->i2c == 0x50005000U) {
		IIC1_CLK_ENABLE();
	} else {
		return -ENOTSUP;
	}

	ret = pinctrl_apply_state(cfg->pcfg, PINCTRL_STATE_DEFAULT);
	if (ret < 0) {
		return ret;
	}

	k_mutex_init(&data->lock);

	I2C_StructInit(&init);
	init.I2C_ClockScale    = I2C_PCLK_8;
	init.I2C_EN            = ENABLE;
	init.I2C_SlaveAddress  = 0x00U;
	init.I2C_Ack           = I2C_IICAA_ACK;
	init.I2C_ICLKDIVH      = I2C_100K_DIVH;
	init.I2C_ICLKDIVL      = I2C_100K_DIVL;
	I2C_Init(cfg->i2c, &init);

	return 0;
}

static DEVICE_API(i2c, i2c_as32x601_api) = {
	.transfer = i2c_as32x601_transfer,
};

#define I2C_AS32X601_INIT(inst)							\
	PINCTRL_DT_INST_DEFINE(inst);						\
	static const struct i2c_as32x601_config i2c_as32x601_config_##inst = {	\
		.i2c        = (I2C_TypeDef *)DT_INST_REG_ADDR(inst),		\
		.clock_freq = DT_INST_PROP(inst, clock_frequency),		\
		.pcfg       = PINCTRL_DT_INST_DEV_CONFIG_GET(inst),		\
	};									\
	static struct i2c_as32x601_data i2c_as32x601_data_##inst;		\
	DEVICE_DT_INST_DEFINE(inst, i2c_as32x601_init, NULL,			\
		&i2c_as32x601_data_##inst, &i2c_as32x601_config_##inst,		\
		POST_KERNEL, CONFIG_I2C_INIT_PRIORITY,				\
		&i2c_as32x601_api);

DT_INST_FOREACH_STATUS_OKAY(I2C_AS32X601_INIT)
