/*
 * Copyright (c) 2024 OneWo-zepLinux Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/fs/fs.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include "ramfs.h"
#include "../vfs_core/vfs_superblock.h"

#define LOG_LEVEL CONFIG_FS_LOG_LEVEL
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(ramfs);

/* Default maximum size for RAMFS (1MB) */
#ifndef CONFIG_RAMFS_MAX_SIZE
#define CONFIG_RAMFS_MAX_SIZE (1024 * 1024)
#endif

/* Forward declarations */
static int ramfs_mount(struct vfs_superblock *sb, const char *source, uint32_t flags);
static int ramfs_unmount(struct vfs_superblock *sb);
static int ramfs_lookup(struct vfs_inode *dir, const char *name, struct vfs_inode **result);
static int ramfs_create(struct vfs_inode *dir, const char *name, mode_t mode,
			struct vfs_inode **result);
static int ramfs_mkdir(struct vfs_inode *dir, const char *name, mode_t mode,
		       struct vfs_inode **result);
static int ramfs_unlink(struct vfs_inode *dir, const char *name);
static int ramfs_rmdir(struct vfs_inode *dir, const char *name);
static int ramfs_open(struct vfs_inode *inode, struct fs_file_t *file);
static int ramfs_close(struct fs_file_t *file);
static ssize_t ramfs_read(struct fs_file_t *file, void *buf, size_t size);
static ssize_t ramfs_write(struct fs_file_t *file, const void *buf, size_t size);
static int ramfs_lseek(struct fs_file_t *file, off_t offset, int whence);
static int ramfs_truncate(struct vfs_inode *inode, off_t length);
static int ramfs_opendir(struct vfs_inode *inode, struct fs_dir_t *dir);
static int ramfs_readdir(struct fs_dir_t *dir, struct fs_dirent *entry);
static int ramfs_closedir(struct fs_dir_t *dir);
static int ramfs_stat(struct vfs_inode *inode, struct fs_dirent *entry);
static int ramfs_statvfs(struct vfs_superblock *sb, struct fs_statvfs *stat);

/* RAMFS driver operations */
static const struct vfs_driver_ops ramfs_ops = {
	.mount = ramfs_mount,
	.unmount = ramfs_unmount,
	.lookup = ramfs_lookup,
	.create = ramfs_create,
	.mkdir = ramfs_mkdir,
	.unlink = ramfs_unlink,
	.rmdir = ramfs_rmdir,
	.open = ramfs_open,
	.close = ramfs_close,
	.read = ramfs_read,
	.write = ramfs_write,
	.lseek = ramfs_lseek,
	.truncate = ramfs_truncate,
	.opendir = ramfs_opendir,
	.readdir = ramfs_readdir,
	.closedir = ramfs_closedir,
	.stat = ramfs_stat,
	.statvfs = ramfs_statvfs,
};

/* RAMFS driver */
static const struct vfs_driver ramfs_driver = {
	.name = "ramfs",
	.type = FS_TYPE_EXTERNAL_BASE,
	.ops = &ramfs_ops,
};

/* Helper: Allocate RAMFS inode data */
static struct ramfs_inode_data *ramfs_inode_data_alloc(void)
{
	struct ramfs_inode_data *data = k_malloc(sizeof(struct ramfs_inode_data));

	if (data != NULL) {
		memset(data, 0, sizeof(struct ramfs_inode_data));
		sys_dlist_init(&data->children);
	}
	return data;
}

/* Helper: Free RAMFS inode data */
static void ramfs_inode_data_free(struct ramfs_inode_data *data)
{
	if (data == NULL) {
		return;
	}

	if (data->data != NULL) {
		k_free(data->data);
	}
	k_free(data);
}

/* Helper: Find directory entry by name */
static struct ramfs_dirent *ramfs_find_dirent(struct vfs_inode *dir, const char *name)
{
	struct ramfs_inode_data *dir_data;
	struct ramfs_dirent *entry;

	if (dir == NULL || name == NULL || !S_ISDIR(dir->mode)) {
		return NULL;
	}

	dir_data = (struct ramfs_inode_data *)dir->fs_data;
	if (dir_data == NULL) {
		return NULL;
	}

	SYS_DLIST_FOR_EACH_CONTAINER(&dir_data->children, entry, node) {
		if (strcmp(entry->name, name) == 0) {
			return entry;
		}
	}

	return NULL;
}

/* Helper: Add directory entry */
static int ramfs_add_dirent(struct vfs_inode *dir, const char *name, struct vfs_inode *inode)
{
	struct ramfs_inode_data *dir_data;
	struct ramfs_dirent *entry;

	if (dir == NULL || name == NULL || inode == NULL || !S_ISDIR(dir->mode)) {
		return -EINVAL;
	}

	/* Check if entry already exists */
	if (ramfs_find_dirent(dir, name) != NULL) {
		return -EEXIST;
	}

	dir_data = (struct ramfs_inode_data *)dir->fs_data;
	if (dir_data == NULL) {
		return -EINVAL;
	}

	entry = k_malloc(sizeof(struct ramfs_dirent));
	if (entry == NULL) {
		return -ENOMEM;
	}

	entry->name = k_malloc(strlen(name) + 1);
	if (entry->name == NULL) {
		k_free(entry);
		return -ENOMEM;
	}
	strcpy(entry->name, name);
	entry->inode = inode;
	vfs_inode_ref(inode);

	sys_dlist_append(&dir_data->children, &entry->node);
	return 0;
}

/* Helper: Remove directory entry */
static int ramfs_remove_dirent(struct vfs_inode *dir, const char *name)
{
	struct ramfs_dirent *entry;

	entry = ramfs_find_dirent(dir, name);
	if (entry == NULL) {
		return -ENOENT;
	}

	sys_dlist_remove(&entry->node);
	vfs_inode_unref(entry->inode);
	k_free(entry->name);
	k_free(entry);

	return 0;
}

static int ramfs_mount(struct vfs_superblock *sb, const char *source, uint32_t flags)
{
	struct ramfs_sb_data *sb_data;
	struct vfs_inode *root_inode;
	struct ramfs_inode_data *root_data;

	LOG_INF("Mounting RAMFS");

	/* Allocate superblock data */
	sb_data = k_malloc(sizeof(struct ramfs_sb_data));
	if (sb_data == NULL) {
		return -ENOMEM;
	}
	sb_data->total_size = 0;
	sb_data->max_size = CONFIG_RAMFS_MAX_SIZE;
	sb->fs_data = sb_data;

	/* Create root inode */
	root_inode = vfs_inode_alloc(sb);
	if (root_inode == NULL) {
		k_free(sb_data);
		return -ENOMEM;
	}

	vfs_inode_init(root_inode, S_IFDIR | 0755, 0, 0);

	/* Allocate root inode data */
	root_data = ramfs_inode_data_alloc();
	if (root_data == NULL) {
		vfs_inode_free(root_inode);
		k_free(sb_data);
		return -ENOMEM;
	}
	root_inode->fs_data = root_data;

	sb->root_inode = root_inode;
	LOG_INF("RAMFS mounted successfully");
	return 0;
}

static int ramfs_unmount(struct vfs_superblock *sb)
{
	struct ramfs_sb_data *sb_data;

	LOG_INF("Unmounting RAMFS");

	if (sb->root_inode != NULL) {
		struct ramfs_inode_data *root_data = sb->root_inode->fs_data;

		if (root_data != NULL) {
			ramfs_inode_data_free(root_data);
		}
		vfs_inode_free(sb->root_inode);
	}

	sb_data = (struct ramfs_sb_data *)sb->fs_data;
	if (sb_data != NULL) {
		k_free(sb_data);
	}

	return 0;
}

static int ramfs_lookup(struct vfs_inode *dir, const char *name, struct vfs_inode **result)
{
	struct ramfs_dirent *entry;

	if (dir == NULL || name == NULL || result == NULL) {
		return -EINVAL;
	}

	if (!S_ISDIR(dir->mode)) {
		return -ENOTDIR;
	}

	entry = ramfs_find_dirent(dir, name);
	if (entry == NULL) {
		return -ENOENT;
	}

	*result = entry->inode;
	vfs_inode_ref(entry->inode);
	return 0;
}

static int ramfs_create(struct vfs_inode *dir, const char *name, mode_t mode,
			struct vfs_inode **result)
{
	struct vfs_inode *inode;
	struct ramfs_inode_data *inode_data;
	int ret;

	if (dir == NULL || name == NULL || result == NULL) {
		return -EINVAL;
	}

	if (!S_ISDIR(dir->mode)) {
		return -ENOTDIR;
	}

	/* Create new inode */
	inode = vfs_inode_alloc(dir->sb);
	if (inode == NULL) {
		return -ENOMEM;
	}

	vfs_inode_init(inode, S_IFREG | (mode & 0777), 0, 0);

	/* Allocate inode data */
	inode_data = ramfs_inode_data_alloc();
	if (inode_data == NULL) {
		vfs_inode_free(inode);
		return -ENOMEM;
	}
	inode->fs_data = inode_data;

	/* Add to directory */
	ret = ramfs_add_dirent(dir, name, inode);
	if (ret < 0) {
		ramfs_inode_data_free(inode_data);
		vfs_inode_free(inode);
		return ret;
	}

	*result = inode;
	LOG_DBG("Created file: %s", name);
	return 0;
}

static int ramfs_mkdir(struct vfs_inode *dir, const char *name, mode_t mode,
		       struct vfs_inode **result)
{
	struct vfs_inode *inode;
	struct ramfs_inode_data *inode_data;
	int ret;

	if (dir == NULL || name == NULL || result == NULL) {
		return -EINVAL;
	}

	if (!S_ISDIR(dir->mode)) {
		return -ENOTDIR;
	}

	/* Create new inode */
	inode = vfs_inode_alloc(dir->sb);
	if (inode == NULL) {
		return -ENOMEM;
	}

	vfs_inode_init(inode, S_IFDIR | (mode & 0777), 0, 0);

	/* Allocate inode data */
	inode_data = ramfs_inode_data_alloc();
	if (inode_data == NULL) {
		vfs_inode_free(inode);
		return -ENOMEM;
	}
	inode->fs_data = inode_data;

	/* Add to directory */
	ret = ramfs_add_dirent(dir, name, inode);
	if (ret < 0) {
		ramfs_inode_data_free(inode_data);
		vfs_inode_free(inode);
		return ret;
	}

	*result = inode;
	LOG_DBG("Created directory: %s", name);
	return 0;
}

static int ramfs_unlink(struct vfs_inode *dir, const char *name)
{
	struct ramfs_dirent *entry;

	if (dir == NULL || name == NULL) {
		return -EINVAL;
	}

	if (!S_ISDIR(dir->mode)) {
		return -ENOTDIR;
	}

	entry = ramfs_find_dirent(dir, name);
	if (entry == NULL) {
		return -ENOENT;
	}

	if (S_ISDIR(entry->inode->mode)) {
		return -EISDIR;
	}

	return ramfs_remove_dirent(dir, name);
}

static int ramfs_rmdir(struct vfs_inode *dir, const char *name)
{
	struct ramfs_dirent *entry;
	struct ramfs_inode_data *inode_data;

	if (dir == NULL || name == NULL) {
		return -EINVAL;
	}

	if (!S_ISDIR(dir->mode)) {
		return -ENOTDIR;
	}

	entry = ramfs_find_dirent(dir, name);
	if (entry == NULL) {
		return -ENOENT;
	}

	if (!S_ISDIR(entry->inode->mode)) {
		return -ENOTDIR;
	}

	/* Check if directory is empty */
	inode_data = (struct ramfs_inode_data *)entry->inode->fs_data;
	if (inode_data != NULL && !sys_dlist_is_empty(&inode_data->children)) {
		return -ENOTEMPTY;
	}

	return ramfs_remove_dirent(dir, name);
}

static int ramfs_open(struct vfs_inode *inode, struct fs_file_t *file)
{
	struct ramfs_file_data *file_data;

	if (inode == NULL || file == NULL) {
		return -EINVAL;
	}

	if (!S_ISREG(inode->mode)) {
		return -EISDIR;
	}

	file_data = k_malloc(sizeof(struct ramfs_file_data));
	if (file_data == NULL) {
		return -ENOMEM;
	}

	file_data->inode = inode;
	file_data->pos = 0;
	vfs_inode_ref(inode);

	file->filep = file_data;
	return 0;
}

static int ramfs_close(struct fs_file_t *file)
{
	struct ramfs_file_data *file_data;

	if (file == NULL || file->filep == NULL) {
		return -EINVAL;
	}

	file_data = (struct ramfs_file_data *)file->filep;
	vfs_inode_unref(file_data->inode);
	k_free(file_data);
	file->filep = NULL;

	return 0;
}

static ssize_t ramfs_read(struct fs_file_t *file, void *buf, size_t size)
{
	struct ramfs_file_data *file_data;
	struct ramfs_inode_data *inode_data;
	size_t to_read;

	if (file == NULL || file->filep == NULL || buf == NULL) {
		return -EINVAL;
	}

	file_data = (struct ramfs_file_data *)file->filep;
	inode_data = (struct ramfs_inode_data *)file_data->inode->fs_data;

	if (inode_data == NULL) {
		return -EINVAL;
	}

	/* Check if position is beyond file size */
	if (file_data->pos >= file_data->inode->size) {
		return 0;
	}

	/* Calculate bytes to read */
	to_read = MIN(size, file_data->inode->size - file_data->pos);

	if (to_read > 0 && inode_data->data != NULL) {
		memcpy(buf, (uint8_t *)inode_data->data + file_data->pos, to_read);
		file_data->pos += to_read;
	}

	return to_read;
}

static ssize_t ramfs_write(struct fs_file_t *file, const void *buf, size_t size)
{
	struct ramfs_file_data *file_data;
	struct ramfs_inode_data *inode_data;
	size_t new_size;
	void *new_data;

	if (file == NULL || file->filep == NULL || buf == NULL) {
		return -EINVAL;
	}

	file_data = (struct ramfs_file_data *)file->filep;
	inode_data = (struct ramfs_inode_data *)file_data->inode->fs_data;

	if (inode_data == NULL) {
		return -EINVAL;
	}

	new_size = file_data->pos + size;

	/* Expand buffer if needed */
	if (new_size > inode_data->data_capacity) {
		size_t new_capacity = MAX(new_size, inode_data->data_capacity * 2);

		new_data = k_realloc(inode_data->data, new_capacity);
		if (new_data == NULL) {
			return -ENOMEM;
		}

		inode_data->data = new_data;
		inode_data->data_capacity = new_capacity;
	}

	memcpy((uint8_t *)inode_data->data + file_data->pos, buf, size);
	file_data->pos += size;

	if (file_data->pos > file_data->inode->size) {
		file_data->inode->size = file_data->pos;
	}

	return size;
}

static int ramfs_lseek(struct fs_file_t *file, off_t offset, int whence)
{
	struct ramfs_file_data *file_data;
	off_t new_pos;

	if (file == NULL || file->filep == NULL) {
		return -EINVAL;
	}

	file_data = (struct ramfs_file_data *)file->filep;

	switch (whence) {
	case FS_SEEK_SET:
		new_pos = offset;
		break;
	case FS_SEEK_CUR:
		new_pos = file_data->pos + offset;
		break;
	case FS_SEEK_END:
		new_pos = file_data->inode->size + offset;
		break;
	default:
		return -EINVAL;
	}

	if (new_pos < 0) {
		return -EINVAL;
	}

	file_data->pos = new_pos;
	return 0;
}

static int ramfs_truncate(struct vfs_inode *inode, off_t length)
{
	struct ramfs_inode_data *inode_data;

	if (inode == NULL || length < 0) {
		return -EINVAL;
	}

	inode_data = (struct ramfs_inode_data *)inode->fs_data;
	if (inode_data == NULL) {
		return -EINVAL;
	}

	if (length == 0) {
		if (inode_data->data != NULL) {
			k_free(inode_data->data);
			inode_data->data = NULL;
			inode_data->data_capacity = 0;
		}
	} else if (length > inode_data->data_capacity) {
		void *new_data = k_realloc(inode_data->data, length);

		if (new_data == NULL) {
			return -ENOMEM;
		}
		inode_data->data = new_data;
		inode_data->data_capacity = length;
	}

	inode->size = length;
	return 0;
}

static int ramfs_opendir(struct vfs_inode *inode, struct fs_dir_t *dir)
{
	struct ramfs_dir_data *dir_data;
	struct ramfs_inode_data *inode_data;

	if (inode == NULL || dir == NULL) {
		return -EINVAL;
	}

	if (!S_ISDIR(inode->mode)) {
		return -ENOTDIR;
	}

	dir_data = k_malloc(sizeof(struct ramfs_dir_data));
	if (dir_data == NULL) {
		return -ENOMEM;
	}

	inode_data = (struct ramfs_inode_data *)inode->fs_data;
	dir_data->inode = inode;
	dir_data->current = sys_dlist_peek_head(&inode_data->children);
	dir_data->entry_count = 0;
	vfs_inode_ref(inode);

	dir->dirp = dir_data;
	return 0;
}

static int ramfs_readdir(struct fs_dir_t *dir, struct fs_dirent *entry)
{
	struct ramfs_dir_data *dir_data;
	struct ramfs_dirent *ramfs_entry;

	if (dir == NULL || dir->dirp == NULL || entry == NULL) {
		return -EINVAL;
	}

	dir_data = (struct ramfs_dir_data *)dir->dirp;

	/* Check if we've reached the end */
	if (dir_data->current == NULL) {
		entry->name[0] = '\0';
		return 0;
	}

	ramfs_entry = CONTAINER_OF(dir_data->current, struct ramfs_dirent, node);

	/* Fill in entry information */
	strncpy(entry->name, ramfs_entry->name, sizeof(entry->name) - 1);
	entry->name[sizeof(entry->name) - 1] = '\0';
	entry->type = S_ISDIR(ramfs_entry->inode->mode) ? FS_DIR_ENTRY_DIR : FS_DIR_ENTRY_FILE;
	entry->size = ramfs_entry->inode->size;

	/* Move to next entry */
	dir_data->current = sys_dlist_peek_next(&((struct ramfs_inode_data *)
						  dir_data->inode->fs_data)->children,
						 dir_data->current);
	dir_data->entry_count++;

	return 0;
}

static int ramfs_closedir(struct fs_dir_t *dir)
{
	struct ramfs_dir_data *dir_data;

	if (dir == NULL || dir->dirp == NULL) {
		return -EINVAL;
	}

	dir_data = (struct ramfs_dir_data *)dir->dirp;
	vfs_inode_unref(dir_data->inode);
	k_free(dir_data);
	dir->dirp = NULL;

	return 0;
}

static int ramfs_stat(struct vfs_inode *inode, struct fs_dirent *entry)
{
	if (inode == NULL || entry == NULL) {
		return -EINVAL;
	}

	entry->type = S_ISDIR(inode->mode) ? FS_DIR_ENTRY_DIR : FS_DIR_ENTRY_FILE;
	entry->size = inode->size;

	return 0;
}

static int ramfs_statvfs(struct vfs_superblock *sb, struct fs_statvfs *stat)
{
	struct ramfs_sb_data *sb_data;

	if (sb == NULL || stat == NULL) {
		return -EINVAL;
	}

	sb_data = (struct ramfs_sb_data *)sb->fs_data;
	if (sb_data == NULL) {
		return -EINVAL;
	}

	memset(stat, 0, sizeof(struct fs_statvfs));
	stat->f_bsize = 1;
	stat->f_frsize = 1;
	stat->f_blocks = sb_data->max_size;
	stat->f_bfree = sb_data->max_size - sb_data->total_size;

	return 0;
}

int ramfs_init(void)
{
	int ret;

	ret = vfs_driver_register(&ramfs_driver);
	if (ret < 0) {
		LOG_ERR("Failed to register RAMFS driver: %d", ret);
		return ret;
	}

	LOG_INF("RAMFS driver initialized");
	return 0;
}

const struct vfs_driver *ramfs_get_driver(void)
{
	return &ramfs_driver;
}

