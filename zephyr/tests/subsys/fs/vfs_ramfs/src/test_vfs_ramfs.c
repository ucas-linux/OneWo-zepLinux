/*
 * Copyright (c) 2024 OneWo-zepLinux Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/ztest.h>
#include <zephyr/kernel.h>
#include <zephyr/fs/fs.h>
#include <string.h>
#include <errno.h>

#define TEST_MOUNT_POINT "/ram"
#define TEST_FILE_PATH "/ram/test.txt"
#define TEST_DIR_PATH "/ram/testdir"
#define TEST_FILE_IN_DIR "/ram/testdir/file.txt"

/* Mount point structure */
static struct fs_mount_t ramfs_mnt = {
	.type = FS_TYPE_EXTERNAL_BASE,
	.mnt_point = TEST_MOUNT_POINT,
	.storage_dev = (void *)"ramfs",
	.flags = 0,
};

/* Test setup - mount filesystem */
static void *vfs_ramfs_setup(void)
{
	int ret;

	printk("Setting up VFS RAMFS test\n");

	/* Mount RAMFS */
	ret = fs_mount(&ramfs_mnt);
	zassert_equal(ret, 0, "Failed to mount RAMFS: %d", ret);

	printk("RAMFS mounted at %s\n", TEST_MOUNT_POINT);
	return NULL;
}

/* Test teardown - unmount filesystem */
static void vfs_ramfs_teardown(void *fixture)
{
	int ret;

	ARG_UNUSED(fixture);

	printk("Tearing down VFS RAMFS test\n");

	/* Unmount RAMFS */
	ret = fs_unmount(&ramfs_mnt);
	zassert_equal(ret, 0, "Failed to unmount RAMFS: %d", ret);

	printk("RAMFS unmounted\n");
}

/* Test: Basic file operations */
ZTEST(vfs_ramfs, test_file_operations)
{
	struct fs_file_t file;
	int ret;
	const char *test_data = "Hello, RAMFS!";
	char read_buf[64];
	ssize_t bytes;

	printk("Testing basic file operations\n");

	fs_file_t_init(&file);

	/* Create and open file */
	ret = fs_open(&file, TEST_FILE_PATH, FS_O_CREATE | FS_O_RDWR);
	zassert_equal(ret, 0, "Failed to create file: %d", ret);

	/* Write data */
	bytes = fs_write(&file, test_data, strlen(test_data));
	zassert_equal(bytes, strlen(test_data), "Write failed: %d", bytes);

	/* Seek to beginning */
	ret = fs_seek(&file, 0, FS_SEEK_SET);
	zassert_equal(ret, 0, "Seek failed: %d", ret);

	/* Read data back */
	memset(read_buf, 0, sizeof(read_buf));
	bytes = fs_read(&file, read_buf, strlen(test_data));
	zassert_equal(bytes, strlen(test_data), "Read failed: %d", bytes);
	zassert_mem_equal(read_buf, test_data, strlen(test_data), "Data mismatch");

	/* Close file */
	ret = fs_close(&file);
	zassert_equal(ret, 0, "Failed to close file: %d", ret);

	printk("File operations test passed\n");
}

/* Test: Directory operations */
ZTEST(vfs_ramfs, test_directory_operations)
{
	struct fs_dir_t dir;
	struct fs_dirent entry;
	int ret;

	printk("Testing directory operations\n");

	fs_dir_t_init(&dir);

	/* Create directory */
	ret = fs_mkdir(TEST_DIR_PATH);
	zassert_equal(ret, 0, "Failed to create directory: %d", ret);

	/* Open directory */
	ret = fs_opendir(&dir, TEST_DIR_PATH);
	zassert_equal(ret, 0, "Failed to open directory: %d", ret);

	/* Read directory (should be empty) */
	ret = fs_readdir(&dir, &entry);
	zassert_equal(ret, 0, "Failed to read directory: %d", ret);
	zassert_equal(entry.name[0], '\0', "Directory should be empty");

	/* Close directory */
	ret = fs_closedir(&dir);
	zassert_equal(ret, 0, "Failed to close directory: %d", ret);

	printk("Directory operations test passed\n");
}

/* Test: File in directory */
ZTEST(vfs_ramfs, test_file_in_directory)
{
	struct fs_file_t file;
	struct fs_dir_t dir;
	struct fs_dirent entry;
	int ret;
	const char *test_data = "File in directory";
	char read_buf[64];
	ssize_t bytes;
	bool found = false;

	printk("Testing file in directory\n");

	fs_file_t_init(&file);
	fs_dir_t_init(&dir);

	/* Create file in directory */
	ret = fs_open(&file, TEST_FILE_IN_DIR, FS_O_CREATE | FS_O_RDWR);
	zassert_equal(ret, 0, "Failed to create file in directory: %d", ret);

	/* Write data */
	bytes = fs_write(&file, test_data, strlen(test_data));
	zassert_equal(bytes, strlen(test_data), "Write failed: %d", bytes);

	/* Close file */
	ret = fs_close(&file);
	zassert_equal(ret, 0, "Failed to close file: %d", ret);

	/* Open directory and verify file exists */
	ret = fs_opendir(&dir, TEST_DIR_PATH);
	zassert_equal(ret, 0, "Failed to open directory: %d", ret);

	/* Read directory entries */
	while (true) {
		ret = fs_readdir(&dir, &entry);
		zassert_equal(ret, 0, "Failed to read directory: %d", ret);

		if (entry.name[0] == '\0') {
			break;
		}

		if (strcmp(entry.name, "file.txt") == 0) {
			found = true;
			zassert_equal(entry.type, FS_DIR_ENTRY_FILE, "Wrong entry type");
			break;
		}
	}

	zassert_true(found, "File not found in directory");

	/* Close directory */
	ret = fs_closedir(&dir);
	zassert_equal(ret, 0, "Failed to close directory: %d", ret);

	/* Reopen and read file */
	ret = fs_open(&file, TEST_FILE_IN_DIR, FS_O_RDWR);
	zassert_equal(ret, 0, "Failed to reopen file: %d", ret);

	memset(read_buf, 0, sizeof(read_buf));
	bytes = fs_read(&file, read_buf, strlen(test_data));
	zassert_equal(bytes, strlen(test_data), "Read failed: %d", bytes);
	zassert_mem_equal(read_buf, test_data, strlen(test_data), "Data mismatch");

	ret = fs_close(&file);
	zassert_equal(ret, 0, "Failed to close file: %d", ret);

	printk("File in directory test passed\n");
}

/* Test: File seek operations */
ZTEST(vfs_ramfs, test_file_seek)
{
	struct fs_file_t file;
	int ret;
	const char *test_data = "0123456789";
	char read_buf[16];
	ssize_t bytes;
	off_t pos;

	printk("Testing file seek operations\n");

	fs_file_t_init(&file);

	/* Create and write file */
	ret = fs_open(&file, TEST_FILE_PATH, FS_O_CREATE | FS_O_RDWR);
	zassert_equal(ret, 0, "Failed to create file: %d", ret);

	bytes = fs_write(&file, test_data, strlen(test_data));
	zassert_equal(bytes, strlen(test_data), "Write failed: %d", bytes);

	/* Test SEEK_SET */
	ret = fs_seek(&file, 5, FS_SEEK_SET);
	zassert_equal(ret, 0, "SEEK_SET failed: %d", ret);

	pos = fs_tell(&file);
	zassert_equal(pos, 5, "Position mismatch after SEEK_SET: %d", pos);

	/* Test SEEK_CUR */
	ret = fs_seek(&file, 2, FS_SEEK_CUR);
	zassert_equal(ret, 0, "SEEK_CUR failed: %d", ret);

	pos = fs_tell(&file);
	zassert_equal(pos, 7, "Position mismatch after SEEK_CUR: %d", pos);

	/* Test SEEK_END */
	ret = fs_seek(&file, -3, FS_SEEK_END);
	zassert_equal(ret, 0, "SEEK_END failed: %d", ret);

	pos = fs_tell(&file);
	zassert_equal(pos, 7, "Position mismatch after SEEK_END: %d", pos);

	/* Read from position */
	memset(read_buf, 0, sizeof(read_buf));
	bytes = fs_read(&file, read_buf, 3);
	zassert_equal(bytes, 3, "Read failed: %d", bytes);
	zassert_mem_equal(read_buf, "789", 3, "Data mismatch");

	ret = fs_close(&file);
	zassert_equal(ret, 0, "Failed to close file: %d", ret);

	printk("File seek test passed\n");
}

/* Test: File truncate */
ZTEST(vfs_ramfs, test_file_truncate)
{
	struct fs_file_t file;
	int ret;
	const char *test_data = "This is a long test string";
	char read_buf[64];
	ssize_t bytes;

	printk("Testing file truncate\n");

	fs_file_t_init(&file);

	/* Create and write file */
	ret = fs_open(&file, TEST_FILE_PATH, FS_O_CREATE | FS_O_RDWR);
	zassert_equal(ret, 0, "Failed to create file: %d", ret);

	bytes = fs_write(&file, test_data, strlen(test_data));
	zassert_equal(bytes, strlen(test_data), "Write failed: %d", bytes);

	/* Truncate file */
	ret = fs_truncate(&file, 10);
	zassert_equal(ret, 0, "Truncate failed: %d", ret);

	/* Seek to beginning and read */
	ret = fs_seek(&file, 0, FS_SEEK_SET);
	zassert_equal(ret, 0, "Seek failed: %d", ret);

	memset(read_buf, 0, sizeof(read_buf));
	bytes = fs_read(&file, read_buf, sizeof(read_buf));
	zassert_equal(bytes, 10, "Read after truncate failed: %d", bytes);
	zassert_mem_equal(read_buf, "This is a ", 10, "Data mismatch after truncate");

	ret = fs_close(&file);
	zassert_equal(ret, 0, "Failed to close file: %d", ret);

	printk("File truncate test passed\n");
}

/* Test: Unlink file */
ZTEST(vfs_ramfs, test_unlink)
{
	struct fs_file_t file;
	int ret;

	printk("Testing file unlink\n");

	fs_file_t_init(&file);

	/* Create file */
	ret = fs_open(&file, TEST_FILE_PATH, FS_O_CREATE | FS_O_RDWR);
	zassert_equal(ret, 0, "Failed to create file: %d", ret);

	ret = fs_close(&file);
	zassert_equal(ret, 0, "Failed to close file: %d", ret);

	/* Unlink file */
	ret = fs_unlink(TEST_FILE_PATH);
	zassert_equal(ret, 0, "Failed to unlink file: %d", ret);

	/* Try to open deleted file - should fail */
	ret = fs_open(&file, TEST_FILE_PATH, FS_O_RDWR);
	zassert_not_equal(ret, 0, "Should not be able to open deleted file");

	printk("File unlink test passed\n");
}

/* Test: Stat operations */
ZTEST(vfs_ramfs, test_stat)
{
	struct fs_file_t file;
	struct fs_dirent stat;
	int ret;
	const char *test_data = "Test data for stat";
	ssize_t bytes;

	printk("Testing stat operations\n");

	fs_file_t_init(&file);

	/* Create and write file */
	ret = fs_open(&file, TEST_FILE_PATH, FS_O_CREATE | FS_O_RDWR);
	zassert_equal(ret, 0, "Failed to create file: %d", ret);

	bytes = fs_write(&file, test_data, strlen(test_data));
	zassert_equal(bytes, strlen(test_data), "Write failed: %d", bytes);

	ret = fs_close(&file);
	zassert_equal(ret, 0, "Failed to close file: %d", ret);

	/* Stat file */
	ret = fs_stat(TEST_FILE_PATH, &stat);
	zassert_equal(ret, 0, "Stat failed: %d", ret);
	zassert_equal(stat.type, FS_DIR_ENTRY_FILE, "Wrong file type");
	zassert_equal(stat.size, strlen(test_data), "Wrong file size: %zu", stat.size);

	/* Stat directory */
	ret = fs_stat(TEST_DIR_PATH, &stat);
	zassert_equal(ret, 0, "Stat directory failed: %d", ret);
	zassert_equal(stat.type, FS_DIR_ENTRY_DIR, "Wrong directory type");

	printk("Stat operations test passed\n");
}

/* Test: Statvfs */
ZTEST(vfs_ramfs, test_statvfs)
{
	struct fs_statvfs stat;
	int ret;

	printk("Testing statvfs\n");

	ret = fs_statvfs(TEST_MOUNT_POINT, &stat);
	zassert_equal(ret, 0, "Statvfs failed: %d", ret);

	printk("Filesystem stats:\n");
	printk("  Block size: %lu\n", stat.f_bsize);
	printk("  Total blocks: %lu\n", stat.f_blocks);
	printk("  Free blocks: %lu\n", stat.f_bfree);

	zassert_true(stat.f_blocks > 0, "Invalid block count");

	printk("Statvfs test passed\n");
}

/* Test suite definition */
ZTEST_SUITE(vfs_ramfs, NULL, vfs_ramfs_setup, NULL, NULL, vfs_ramfs_teardown);
