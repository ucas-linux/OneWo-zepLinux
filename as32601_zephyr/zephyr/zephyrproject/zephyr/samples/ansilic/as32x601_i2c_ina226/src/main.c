#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>

#define INA226_NODE  DT_NODELABEL(ina226)
#define INA226_ADDR  DT_REG_ADDR(INA226_NODE)

#define INA226_REG_CONFIG  0x00U
#define INA226_REG_SHUNTV  0x01U
#define INA226_REG_BUSV    0x02U
#define INA226_REG_POWER   0x03U
#define INA226_REG_CURRENT 0x04U
#define INA226_REG_CAL     0x05U

#define INA226_CONFIG_VAL  0x4127U
#define INA226_CAL_VAL     512U

static const struct device *i2c_dev = DEVICE_DT_GET(DT_BUS(INA226_NODE));

static int ina226_write_reg(uint8_t reg, uint16_t val)
{
	uint8_t buf[3] = {reg, (uint8_t)(val >> 8U), (uint8_t)(val & 0xFFU)};

	return i2c_write(i2c_dev, buf, sizeof(buf), INA226_ADDR);
}

static int ina226_read_reg(uint8_t reg, uint16_t *val)
{
	uint8_t rx[2];
	int ret = i2c_write_read(i2c_dev, INA226_ADDR, &reg, 1U, rx, sizeof(rx));

	if (ret == 0) {
		*val = ((uint16_t)rx[0] << 8U) | rx[1];
	}
	return ret;
}

int main(void)
{
	if (!device_is_ready(i2c_dev)) {
		printk("I2C device not ready\n");
		return -1;
	}

	/* Configure: 4 averages, 1.1ms conversion, continuous mode */
	int ret = ina226_write_reg(INA226_REG_CONFIG, INA226_CONFIG_VAL);

	if (ret != 0) {
		printk("INA226 config error %d\n", ret);
		return ret;
	}

	/* Calibrate for 1mA/LSB with 10mΩ shunt */
	ret = ina226_write_reg(INA226_REG_CAL, INA226_CAL_VAL);
	if (ret != 0) {
		printk("INA226 cal error %d\n", ret);
		return ret;
	}

	while (1) {
		uint16_t bus_raw, current_raw, power_raw;
		int ret_b = ina226_read_reg(INA226_REG_BUSV, &bus_raw);
		int ret_c = ina226_read_reg(INA226_REG_CURRENT, &current_raw);
		int ret_p = ina226_read_reg(INA226_REG_POWER, &power_raw);

		if (ret_b != 0 || ret_c != 0 || ret_p != 0) {
			printk("INA226 read error\n");
		} else {
			/* Bus voltage: LSB = 1.25 mV → multiply raw by 125, divide by 100 for mV */
			uint32_t bus_mv = (uint32_t)bus_raw * 125U / 100U;
			/* Current: signed, LSB = 1 mA */
			int32_t current_ma = (int16_t)current_raw;
			bool    cur_neg = current_ma < 0;
			uint32_t cur_abs_ma = (uint32_t)(cur_neg ? -current_ma : current_ma);
			/* Power: LSB = 25 mW */
			uint32_t power_mw = (uint32_t)power_raw * 25U;

			printk("[INA226] V=%u.%03uV  I=%s%u.%03uA  P=%u.%03uW\n",
			       bus_mv / 1000U, bus_mv % 1000U,
			       cur_neg ? "-" : "",
			       cur_abs_ma / 1000U, cur_abs_ma % 1000U,
			       power_mw / 1000U, power_mw % 1000U);
		}

		k_sleep(K_SECONDS(1));
	}

	return 0;
}
