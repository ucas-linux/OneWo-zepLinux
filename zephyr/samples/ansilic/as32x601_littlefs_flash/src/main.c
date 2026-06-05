/*
 * Copyright (c) 2024 OneWo-zepLinux Project
 * SPDX-License-Identifier: Apache-2.0
 *
 * LittleFS on QSPI Flash Demo
 * This demo shows how to use LittleFS filesystem on QSPI Flash without Flash Map.
 * It uses a custom backend that directly accesses the Flash device.
 */

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/fs/fs.h>
#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>
#include <string.h>
#include <errno.h>
#include <lfs.h>

#define QSPI_FLASH_NODE DT_ALIAS(qspi_flash)
#define MOUNT_POINT "/lfs"

/* Partition configuration on Flash */
#define PARTITION_OFFSET 0x200000  /* 2MB offset */
#define PARTITION_SIZE   0x100000  /* 1MB size */
#define BLOCK_SIZE       0x1000    /* 4KB block size */

/* Flash device */
static const struct device *flash_dev = DEVICE_DT_GET(QSPI_FLASH_NODE);

/* LittleFS backend functions */
static int lfs_read(const struct lfs_config *c, lfs_block_t block,
		    lfs_off_t off, void *buffer, lfs_size_t size)
{
	uint32_t addr = PARTITION_OFFSET + (block * c->block_size) + off;
	return flash_read(flash_dev, addr, buffer, size);
}

static int lfs_prog(const struct lfs_config *c, lfs_block_t block,
		    lfs_off_t off, const void *buffer, lfs_size_t size)
{
	uint32_t addr = PARTITION_OFFSET + (block * c->block_size) + off;
	return flash_write(flash_dev, addr, buffer, size);
}

static int lfs_erase(const struct lfs_config *c, lfs_block_t block)
{
	uint32_t addr = PARTITION_OFFSET + (block * c->block_size);
	return flash_erase(flash_dev, addr, c->block_size);
}

static int lfs_sync(const struct lfs_config *c)
{
	return 0;
}

/* LittleFS configuration */
static uint8_t read_buf[256];
static uint8_t prog_buf[256];
static uint8_t lookahead_buf[16];

static struct lfs_config lfs_cfg = {
	.read = lfs_read,
	.prog = lfs_prog,
	.erase = lfs_erase,
	.sync = lfs_sync,
	.read_size = 16,
	.prog_size = 16,
	.block_size = BLOCK_SIZE,
	.block_count = PARTITION_SIZE / BLOCK_SIZE,
	.cache_size = 256,
	.lookahead_size = 16,
	.block_cycles = 500,
	.read_buffer = read_buf,
	.prog_buffer = prog_buf,
	.lookahead_buffer = lookahead_buf,
};

static lfs_t lfs;

/* Helper function to list directory contents */
static void list_dir(const char *path)
{
	lfs_dir_t dir;
	struct lfs_info info;
	int ret;

	printk("\n=== Directory: %s ===\n", path);

	ret = lfs_dir_open(&lfs, &dir, path);
	if (ret < 0) {
		printk("Failed to open directory: %d\n", ret);
		return;
	}

	while (true) {
		ret = lfs_dir_read(&lfs, &dir, &info);
		if (ret < 0) {
			printk("Error reading directory: %d\n", ret);
			break;
		}
		if (ret == 0) {
			break;
		}

		if (strcmp(info.name, ".") == 0 || strcmp(info.name, "..") == 0) {
			continue;
		}

		if (info.type == LFS_TYPE_DIR) {
			printk("[DIR]  %s/\n", info.name);
		} else {
			printk("[FILE] %s (%d bytes)\n", info.name, (int)info.size);
		}
	}

	lfs_dir_close(&lfs, &dir);
	printk("=== End ===\n\n");
}

/* Test: Basic file operations */
static int test_file_operations(void)
{
	lfs_file_t file;
	int ret;
	const char *test_data = "Hello, LittleFS on QSPI Flash!";
	char read_buf[128];
	static uint8_t file_buffer[256];
	struct lfs_file_config file_cfg = {
		.buffer = file_buffer,
	};

	printk("\n=== Test 1: Basic file operations ===\n");

	/* Open file for writing */
	ret = lfs_file_opencfg(&lfs, &file, "/test.txt", LFS_O_RDWR | LFS_O_CREAT, &file_cfg);
	if (ret < 0) {
		printk("Failed to open file: %d\n", ret);
		return ret;
	}

	/* Write data */
	ret = lfs_file_write(&lfs, &file, test_data, strlen(test_data));
	if (ret < 0) {
		printk("Write failed: %d\n", ret);
		lfs_file_close(&lfs, &file);
		return ret;
	}
	printk("Wrote %d bytes\n", ret);

	/* Seek to beginning */
	ret = lfs_file_seek(&lfs, &file, 0, LFS_SEEK_SET);
	if (ret < 0) {
		printk("Seek failed: %d\n", ret);
		lfs_file_close(&lfs, &file);
		return ret;
	}

	/* Read back */
	memset(read_buf, 0, sizeof(read_buf));
	ret = lfs_file_read(&lfs, &file, read_buf, strlen(test_data));
	if (ret < 0) {
		printk("Read failed: %d\n", ret);
		lfs_file_close(&lfs, &file);
		return ret;
	}

	if (memcmp(read_buf, test_data, strlen(test_data)) != 0) {
		printk("Data mismatch!\n");
		lfs_file_close(&lfs, &file);
		return -EIO;
	}
	printk("Read back: %s\n", read_buf);

	lfs_file_close(&lfs, &file);
	printk("Test 1 PASSED\n");
	return 0;
}

/* Test: Directory operations */
static int test_directory_operations(void)
{
	int ret;
	static uint8_t file_buffer1[256];
	static uint8_t file_buffer2[256];
	struct lfs_file_config file_cfg1 = {
		.buffer = file_buffer1,
	};
	struct lfs_file_config file_cfg2 = {
		.buffer = file_buffer2,
	};

	printk("\n=== Test 2: Directory operations ===\n");

	/* Create directories */
	ret = lfs_mkdir(&lfs, "/docs");
	if (ret < 0 && ret != LFS_ERR_EXIST) {
		printk("Failed to create /docs: %d\n", ret);
		return ret;
	}

	ret = lfs_mkdir(&lfs, "/config");
	if (ret < 0 && ret != LFS_ERR_EXIST) {
		printk("Failed to create /config: %d\n", ret);
		return ret;
	}

	ret = lfs_mkdir(&lfs, "/data");
	if (ret < 0 && ret != LFS_ERR_EXIST) {
		printk("Failed to create /data: %d\n", ret);
		return ret;
	}

	printk("Created directory structure\n");

	/* Create files in directories */
	lfs_file_t file;

	ret = lfs_file_opencfg(&lfs, &file, "/docs/readme.txt", LFS_O_RDWR | LFS_O_CREAT, &file_cfg1);
	if (ret == 0) {
		const char *content = "This is a README file";
		lfs_file_write(&lfs, &file, content, strlen(content));
		lfs_file_close(&lfs, &file);
		printk("Created /docs/readme.txt\n");
	}

	ret = lfs_file_opencfg(&lfs, &file, "/config/settings.ini", LFS_O_RDWR | LFS_O_CREAT, &file_cfg2);
	if (ret == 0) {
		const char *content = "[Settings]\nversion=1.0\n";
		lfs_file_write(&lfs, &file, content, strlen(content));
		lfs_file_close(&lfs, &file);
		printk("Created /config/settings.ini\n");
	}

	printk("Test 2 PASSED\n");
	return 0;
}

int main(void)
{
	int ret;

	printk("\n");
	printk("========================================\n");
	printk("AS32X601 LittleFS on QSPI Flash Demo\n");
	printk("========================================\n\n");

	/* Check flash device */
	if (!device_is_ready(flash_dev)) {
		printk("ERROR: Flash device not ready\n");
		return -ENODEV;
	}
	printk("Flash device ready\n");

	/* Mount filesystem */
	printk("Mounting LittleFS...\n");
	ret = lfs_mount(&lfs, &lfs_cfg);
	if (ret < 0) {
		printk("Mount failed: %d\n", ret);
		printk("Formatting filesystem...\n");
		ret = lfs_format(&lfs, &lfs_cfg);
		if (ret < 0) {
			printk("Format failed: %d\n", ret);
			return ret;
		}
		printk("Format successful\n");

		ret = lfs_mount(&lfs, &lfs_cfg);
		if (ret < 0) {
			printk("Mount after format failed: %d\n", ret);
			return ret;
		}
	}
	printk("Filesystem mounted successfully\n");

	/* Run tests */
	ret = test_file_operations();
	if (ret != 0) {
		goto cleanup;
	}

	ret = test_directory_operations();
	if (ret != 0) {
		goto cleanup;
	}

	/* List root directory */
	list_dir("/");

	printk("\n========================================\n");
	printk("All tests PASSED!\n");
	printk("========================================\n\n");

cleanup:
	if (ret != 0) {
		printk("\n========================================\n");
		printk("Tests FAILED with error: %d\n", ret);
		printk("========================================\n\n");
	}

	/* Unmount filesystem */
	printk("Unmounting filesystem...\n");
	lfs_unmount(&lfs);
	printk("Done\n");

	return ret;
}
