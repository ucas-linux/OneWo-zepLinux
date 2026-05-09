#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/i2c.h>
#include <string.h>

#define EEPROM_NODE    DT_NODELABEL(eeprom)
#define EEPROM_ADDR    DT_REG_ADDR(EEPROM_NODE)
#define TEST_ADDR      0x00U
#define DATA_LEN       16U
#define PAGE_SIZE      8U
#define WRITE_CYCLE_MS 10U

static const struct device *i2c_dev = DEVICE_DT_GET(DT_BUS(EEPROM_NODE));

static void print_buffer(const char *label, const uint8_t *data, uint8_t len)
{
	printk("%s:", label);
	for (uint8_t i = 0U; i < len; i++) {
		printk(" %02x", data[i]);
	}
	printk("\n");
}

static int eeprom_write_page(uint8_t mem_addr, const uint8_t *data, uint8_t len)
{
	uint8_t buf[PAGE_SIZE + 1U];

	buf[0] = mem_addr;
	memcpy(&buf[1], data, len);
	return i2c_write(i2c_dev, buf, 1U + len, EEPROM_ADDR);
}

static int eeprom_read(uint8_t mem_addr, uint8_t *data, uint8_t len)
{
	return i2c_write_read(i2c_dev, EEPROM_ADDR, &mem_addr, 1U, data, len);
}

int main(void)
{
	uint8_t write_data[DATA_LEN];
	uint8_t read_data[DATA_LEN];
	uint8_t iteration = 0U;

	if (!device_is_ready(i2c_dev)) {
		printk("I2C device not ready\n");
		return -1;
	}

	while (1) {
		for (int i = 0; i < DATA_LEN; i++) {
			write_data[i] = (uint8_t)(iteration + i);
		}

		int ret = eeprom_write_page(TEST_ADDR, write_data, PAGE_SIZE);

		if (ret == 0) {
			k_msleep(WRITE_CYCLE_MS);
			ret = eeprom_write_page(TEST_ADDR + PAGE_SIZE, &write_data[PAGE_SIZE], PAGE_SIZE);
		}
		if (ret != 0) {
			printk("FAIL: write error %d\n", ret);
			k_sleep(K_SECONDS(5));
			iteration++;
			continue;
		}
		k_msleep(WRITE_CYCLE_MS);

		ret = eeprom_read(TEST_ADDR, read_data, DATA_LEN);
		if (ret != 0) {
			printk("FAIL: read error %d\n", ret);
			k_sleep(K_SECONDS(5));
			iteration++;
			continue;
		}

		bool pass = true;

		for (int i = 0; i < DATA_LEN; i++) {
			if (read_data[i] != write_data[i]) {
				printk("FAIL at byte %d\n", i);
				pass = false;
				break;
			}
		}
		print_buffer("EEPROM write", write_data, DATA_LEN);
		print_buffer("EEPROM read ", read_data, DATA_LEN);
		if (pass) {
			printk("EEPROM test PASS\n");
		}

		iteration++;
		k_sleep(K_SECONDS(5));
	}

	return 0;
}
