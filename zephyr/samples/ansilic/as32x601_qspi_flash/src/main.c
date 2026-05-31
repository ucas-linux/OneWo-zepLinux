#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/__assert.h>
#include <string.h>

#define QSPI_FLASH_NODE DT_ALIAS(qspi_flash)
#define TEST_SECTOR_SIZE 0x40000U
#define TEST_PAGE_SIZE   256U

static uint8_t write_buf[TEST_PAGE_SIZE] __aligned(4);
static uint8_t read_buf[TEST_PAGE_SIZE] __aligned(4);

int main(void)
{
	const struct device *flash = DEVICE_DT_GET(QSPI_FLASH_NODE);
	const struct flash_parameters *params;
	struct flash_pages_info info;
	int ret;

	printk("AS32X601 QSPI flash test start\n");

	if (!device_is_ready(flash)) {
		printk("flash device not ready\n");
		return -ENODEV;
	}

	params = flash_get_parameters(flash);
	printk("flash parameters:\n");
	printk("  write_block_size = %u\n", (unsigned int)params->write_block_size);
	printk("  erase_value      = 0x%02x\n", params->erase_value);

	ret = flash_get_page_info_by_offs(flash, 0, &info);
	if (ret != 0) {
		printk("get page info failed (%d)\n", ret);
		return ret;
	}
	printk("page layout at offset 0x0:\n");
	printk("  start_offset = 0x%lx\n", (unsigned long)info.start_offset);
	printk("  size         = 0x%zx\n", info.size);
	printk("  index        = %u\n", (unsigned int)info.index);

	printk("erase sector @ 0x0 (size 0x%x)...\n", TEST_SECTOR_SIZE);
	ret = flash_erase(flash, 0, TEST_SECTOR_SIZE);
	if (ret != 0) {
		printk("erase failed (%d)\n", ret);
		return ret;
	}

	ret = flash_read(flash, 0, read_buf, TEST_PAGE_SIZE);
	if (ret != 0) {
		printk("read after erase failed (%d)\n", ret);
		return ret;
	}
	for (size_t i = 0; i < TEST_PAGE_SIZE; i++) {
		if (read_buf[i] != 0xFF) {
			printk("erase verify FAILED at %zu: 0x%02x\n", i, read_buf[i]);
			return -EIO;
		}
	}
	printk("erase verify ok\n");

	for (size_t i = 0; i < TEST_PAGE_SIZE; i++) {
		write_buf[i] = (uint8_t)i;
	}
	printk("write %u bytes @ 0x0...\n", TEST_PAGE_SIZE);
	ret = flash_write(flash, 0, write_buf, TEST_PAGE_SIZE);
	if (ret != 0) {
		printk("write failed (%d)\n", ret);
		return ret;
	}

	ret = flash_read(flash, 0, read_buf, TEST_PAGE_SIZE);
	if (ret != 0) {
		printk("read after write failed (%d)\n", ret);
		return ret;
	}
	if (memcmp(write_buf, read_buf, TEST_PAGE_SIZE) != 0) {
		printk("write/read verify FAILED\n");
		return -EIO;
	}
	printk("write/read verify ok\n");

	/* Test 2: Multiple page writes */
	printk("\n=== Test 2: Multiple page writes ===\n");
	for (int page = 0; page < 4; page++) {
		uint32_t offset = page * TEST_PAGE_SIZE;
		for (size_t i = 0; i < TEST_PAGE_SIZE; i++) {
			write_buf[i] = (uint8_t)(page * 16 + i);
		}
		printk("write page %d @ 0x%x...\n", page, offset);
		ret = flash_write(flash, offset, write_buf, TEST_PAGE_SIZE);
		if (ret != 0) {
			printk("write page %d failed (%d)\n", page, ret);
			return ret;
		}
	}

	/* Verify all pages */
	for (int page = 0; page < 4; page++) {
		uint32_t offset = page * TEST_PAGE_SIZE;
		ret = flash_read(flash, offset, read_buf, TEST_PAGE_SIZE);
		if (ret != 0) {
			printk("read page %d failed (%d)\n", page, ret);
			return ret;
		}
		for (size_t i = 0; i < TEST_PAGE_SIZE; i++) {
			uint8_t expected = (uint8_t)(page * 16 + i);
			if (read_buf[i] != expected) {
				printk("page %d verify FAILED at %zu: got 0x%02x, expected 0x%02x\n",
				       page, i, read_buf[i], expected);
				return -EIO;
			}
		}
		printk("page %d verify ok\n", page);
	}

	/* Test 3: Partial page read/write */
	printk("\n=== Test 3: Partial page operations ===\n");
	uint32_t partial_offset = 0x1000;
	uint32_t partial_size = 64;

	printk("erase sector @ 0x%x...\n", partial_offset);
	ret = flash_erase(flash, partial_offset, TEST_SECTOR_SIZE);
	if (ret != 0) {
		printk("erase failed (%d)\n", ret);
		return ret;
	}

	for (size_t i = 0; i < partial_size; i++) {
		write_buf[i] = 0xAA + i;
	}
	printk("write %u bytes @ 0x%x...\n", partial_size, partial_offset);
	ret = flash_write(flash, partial_offset, write_buf, partial_size);
	if (ret != 0) {
		printk("partial write failed (%d)\n", ret);
		return ret;
	}

	ret = flash_read(flash, partial_offset, read_buf, partial_size);
	if (ret != 0) {
		printk("partial read failed (%d)\n", ret);
		return ret;
	}
	if (memcmp(write_buf, read_buf, partial_size) != 0) {
		printk("partial write/read verify FAILED\n");
		return -EIO;
	}
	printk("partial write/read verify ok\n");

	/* Test 4: Boundary test - write across page boundary */
	printk("\n=== Test 4: Cross-page boundary write ===\n");
	uint32_t boundary_offset = TEST_PAGE_SIZE - 32;
	uint32_t boundary_size = 64; /* crosses page boundary */

	for (size_t i = 0; i < boundary_size; i++) {
		write_buf[i] = 0x55 + i;
	}
	printk("write %u bytes @ 0x%x (crosses page boundary)...\n",
	       boundary_size, boundary_offset);
	ret = flash_write(flash, boundary_offset, write_buf, boundary_size);
	if (ret != 0) {
		printk("boundary write failed (%d)\n", ret);
		return ret;
	}

	ret = flash_read(flash, boundary_offset, read_buf, boundary_size);
	if (ret != 0) {
		printk("boundary read failed (%d)\n", ret);
		return ret;
	}
	if (memcmp(write_buf, read_buf, boundary_size) != 0) {
		printk("boundary write/read verify FAILED\n");
		return -EIO;
	}
	printk("boundary write/read verify ok\n");

	__ASSERT(params->write_block_size > 0, "write_block_size must be non-zero");
	__ASSERT(params->erase_value == 0xFF, "NOR flash erase_value must be 0xFF");
	__ASSERT(info.size == TEST_SECTOR_SIZE, "sector size must be 256KB");

	printk("\n=== All tests passed ===\n");
	printk("AS32X601 QSPI flash test done\n");
	return 0;
}
