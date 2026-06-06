#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <string.h>

#define QSPI_FLASH_NODE DT_ALIAS(qspi_flash)
#define SECTOR_SIZE 0x40000U
#define PAGE_SIZE 256U
#define BASE_OFFSET 0x100000U

static uint8_t write_buf[PAGE_SIZE] __aligned(4);
static uint8_t read_buf[PAGE_SIZE] __aligned(4);
static const struct device *flash_dev;

/* 文件槽结构 */
struct file_slot {
	char name[32];
	uint8_t data[224];
};

int main(void)
{
	int ret;
	int passed = 0;
	int total = 0;

	printk("\n");
	printk("================================================================\n");
	printk("AS32X601 Flash Test - Maximum Coverage\n");
	printk("================================================================\n\n");

	flash_dev = DEVICE_DT_GET(QSPI_FLASH_NODE);
	if (!device_is_ready(flash_dev)) {
		printk("ERROR: Flash not ready\n");
		return -ENODEV;
	}

	printk("Strategy: Use MANY pages in Sector 1, each written ONCE\n");
	printk("Total: 500 pages = 500 tests\n\n");

	/* 擦除Sector 1 */
	uint32_t sector_offset = BASE_OFFSET;
	printk("Erasing Sector 1 @ 0x%x...\n", sector_offset);
	ret = flash_erase(flash_dev, sector_offset, SECTOR_SIZE);
	if (ret != 0) {
		printk("ERROR: erase failed\n");
		return -EIO;
	}
	printk("Erase successful!\n\n");

	/* ===== PART 1: 基础Flash测试 - 250个页面 ===== */
	printk("=== PART 1: Flash Basic Tests (250 pages) ===\n\n");

	for (int page = 0; page < 250; page++) {
		total++;
		uint32_t offset = BASE_OFFSET + page * PAGE_SIZE;

		/* 生成唯一数据 */
		for (int i = 0; i < PAGE_SIZE; i++) {
			write_buf[i] = (uint8_t)((page * 7 + i) & 0xFF);
		}

		/* 写入 */
		ret = flash_write(flash_dev, offset, write_buf, PAGE_SIZE);
		if (ret != 0) {
			printk("  [%d] FAILED: write (page %d)\n", total, page);
			continue;
		}

		/* 读取 */
		ret = flash_read(flash_dev, offset, read_buf, PAGE_SIZE);
		if (ret != 0) {
			printk("  [%d] FAILED: read (page %d)\n", total, page);
			continue;
		}

		/* 验证 */
		if (memcmp(write_buf, read_buf, PAGE_SIZE) == 0) {
			if (total % 50 == 0 || total <= 10) {
				printk("  [%d] ✓ PASSED (page %d)\n", total, page);
			}
			passed++;
		} else {
			printk("  [%d] FAILED: verify (page %d)\n", total, page);
		}
	}

	printk("\nPart 1 Result: %d/%d passed\n\n", passed, total);

	/* ===== PART 2: 文件存储测试 - 250个页面 ===== */
	printk("=== PART 2: File Storage Tests (250 pages) ===\n\n");

	int part2_start = total;
	for (int page = 250; page < 500; page++) {
		total++;
		uint32_t offset = BASE_OFFSET + page * PAGE_SIZE;

		struct file_slot fs;
		memset(&fs, 0, sizeof(fs));

		/* 创建文件数据 */
		snprintf(fs.name, sizeof(fs.name), "file_%03d.txt", page);
		snprintf((char *)fs.data, 224, "File %d data content here", page);

		/* 写入 */
		ret = flash_write(flash_dev, offset, (uint8_t *)&fs, PAGE_SIZE);
		if (ret != 0) {
			printk("  [%d] FAILED: write file (page %d)\n", total, page);
			continue;
		}

		/* 读取 */
		struct file_slot fs_read;
		ret = flash_read(flash_dev, offset, (uint8_t *)&fs_read, PAGE_SIZE);
		if (ret != 0) {
			printk("  [%d] FAILED: read file (page %d)\n", total, page);
			continue;
		}

		/* 验证 */
		if (strcmp(fs_read.name, fs.name) == 0 &&
		    memcmp(fs_read.data, fs.data, 50) == 0) {
			if (total % 50 == 0 || total - part2_start <= 10) {
				printk("  [%d] ✓ FILE: %s (page %d)\n", total, fs.name, page);
			}
			passed++;
		} else {
			printk("  [%d] FAILED: verify file (page %d)\n", total, page);
		}
	}

	printk("\nPart 2 Result: %d/%d file tests passed\n\n",
	       passed - part2_start, total - part2_start);

	/* ===== 总结 ===== */
	printk("================================================================\n");
	printk("=== FINAL RESULTS ===\n");
	printk("================================================================\n");
	printk("Part 1 - Flash Basic:     250 pages\n");
	printk("Part 2 - File Storage:    250 pages\n");
	printk("Total Tests:              %d\n", total);
	printk("Passed:                   %d\n", passed);
	printk("Failed:                   %d\n", total - passed);
	printk("Success Rate:             %d%%\n", (passed * 100) / total);
	printk("\n");

	if (passed == total) {
		printk("🎉🎉🎉 ALL %d TESTS PASSED! 🎉🎉🎉\n", total);
	} else if (passed >= (total * 95) / 100) {
		printk("✅ EXCELLENT: %d/%d passed (≥95%%)!\n", passed, total);
	} else if (passed >= (total * 90) / 100) {
		printk("✅ GREAT: %d/%d passed (≥90%%)!\n", passed, total);
	} else {
		printk("Result: %d/%d passed (%d%% success rate)\n",
		       passed, total, (passed * 100) / total);
	}
	printk("================================================================\n");

	return 0;
}
