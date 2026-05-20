/*
 * Copyright (c) 2024 OneWo-zepLinux Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/fs/fs.h>
#include <zephyr/fs/fs_sys.h>
#include <zephyr/posix/sys/stat.h>
#include <errno.h>
#include <string.h>
#include "vfs_adapter.h"
#include "vfs_inode.h"
#include "vfs_superblock.h"
#include "../ramfs/ramfs.h"
#include "../fs_impl.h"

#define LOG_LEVEL CONFIG_FS_LOG_LEVEL
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(vfs_adapter);

/* Adapter context stored in mount point */
struct vfs_adapter_ctx {
	struct vfs_superblock *sb;
	const struct vfs_driver *driver;
};

/* Forward declarations */
static int vfs_adapter_mount(struct fs_mount_t *mountp);
static int vfs_adapter_unmount(struct fs_mount_t *mountp);
static int vfs_adapter_unlink(struct fs_mount_t *mountp, const char *name);
static int vfs_adapter_rename(struct fs_mount_t *mountp, const char *from, const char *to);
static int vfs_adapter_mkdir(struct fs_mount_t *mountp, const char *name);
static int vfs_adapter_opendir(struct fs_dir_t *dirp, const char *fs_path);
static int vfs_adapter_readdir(struct fs_dir_t *dirp, struct fs_dirent *entry);
static int vfs_adapter_closedir(struct fs_dir_t *dirp);
static int vfs_adapter_open(struct fs_file_t *filp, const char *fs_path, fs_mode_t flags);
static int vfs_adapter_close(struct fs_file_t *filp);
static ssize_t vfs_adapter_read(struct fs_file_t *filp, void *dest, size_t nbytes);
static ssize_t vfs_adapter_write(struct fs_file_t *filp, const void *src, size_t nbytes);
static int vfs_adapter_lseek(struct fs_file_t *filp, off_t off, int whence);
static off_t vfs_adapter_tell(struct fs_file_t *filp);
static int vfs_adapter_truncate(struct fs_file_t *filp, off_t length);
static int vfs_adapter_sync(struct fs_file_t *filp);
static int vfs_adapter_stat(struct fs_mount_t *mountp, const char *path, struct fs_dirent *entry);
static int vfs_adapter_statvfs(struct fs_mount_t *mountp, const char *path,
			       struct fs_statvfs *stat);

/* Helper: Parse path and lookup inode */
static int vfs_adapter_lookup_path(struct vfs_superblock *sb, const char *path,
				   struct vfs_inode **parent, struct vfs_inode **inode,
				   char **name)
{
	char *path_copy, *token, *last_token = NULL;
	char *saveptr;
	struct vfs_inode *current, *next;
	int ret = 0;

	if (sb == NULL || path == NULL) {
		return -EINVAL;
	}

	/* Start from root */
	current = sb->root_inode;
	vfs_inode_ref(current);

	/* Handle root path */
	if (strcmp(path, "/") == 0) {
		*inode = current;
		if (parent) {
			*parent = NULL;
		}
		if (name) {
			*name = NULL;
		}
		return 0;
	}

	/* Make a copy of path for tokenization */
	path_copy = k_malloc(strlen(path) + 1);
	if (path_copy == NULL) {
		vfs_inode_unref(current);
		return -ENOMEM;
	}
	strcpy(path_copy, path);

	/* Remove leading slash */
	char *p = path_copy;
	if (*p == '/') {
		p++;
	}

	/* Traverse path */
	token = strtok_r(p, "/", &saveptr);
	while (token != NULL) {
		if (!S_ISDIR(current->mode)) {
			ret = -ENOTDIR;
			goto cleanup;
		}

		last_token = token;
		token = strtok_r(NULL, "/", &saveptr);

		/* If this is the last token and we need the parent, stop here */
		if (token == NULL && parent != NULL) {
			break;
		}

		/* Lookup next component */
		ret = sb->driver->ops->lookup(current, last_token, &next);
		if (ret < 0) {
			goto cleanup;
		}

		vfs_inode_unref(current);
		current = next;
	}

	/* Set return values */
	if (parent != NULL && last_token != NULL) {
		/* Caller wants parent + name (for create/mkdir/unlink) */
		*parent = current;
		if (name && last_token) {
			*name = k_malloc(strlen(last_token) + 1);
			if (*name) {
				strcpy(*name, last_token);
			}
		}
		if (inode) {
			/* Try to lookup the final component */
			ret = sb->driver->ops->lookup(current, last_token, inode);
			if (ret < 0 && ret != -ENOENT) {
				vfs_inode_unref(current);
			}
		}
	} else if (parent == NULL && inode != NULL) {
		/* Caller wants the resolved inode directly */
		*inode = current;
	} else {
		*inode = current;
		if (parent) {
			*parent = NULL;
		}
		if (name) {
			*name = NULL;
		}
	}

cleanup:
	k_free(path_copy);
	return ret;
}

static int vfs_adapter_mount(struct fs_mount_t *mountp)
{
	struct vfs_adapter_ctx *ctx;
	struct vfs_superblock *sb;
	const struct vfs_driver *driver;
	int ret;

	LOG_INF("VFS adapter mount: %s", mountp->mnt_point);

	/* Get driver from mount point storage_dev (used as driver name) */
	if (mountp->storage_dev == NULL) {
		LOG_ERR("No driver specified");
		return -EINVAL;
	}

	driver = vfs_driver_find((const char *)mountp->storage_dev);
	if (driver == NULL) {
		LOG_ERR("Driver not found: %s", (const char *)mountp->storage_dev);
		return -ENODEV;
	}

	/* Allocate adapter context */
	ctx = k_malloc(sizeof(struct vfs_adapter_ctx));
	if (ctx == NULL) {
		return -ENOMEM;
	}

	/* Allocate superblock */
	sb = vfs_sb_alloc(driver, mountp->mnt_point);
	if (sb == NULL) {
		k_free(ctx);
		return -ENOMEM;
	}

	/* Call driver mount */
	ret = driver->ops->mount(sb, NULL, mountp->flags);
	if (ret < 0) {
		vfs_sb_free(sb);
		k_free(ctx);
		return ret;
	}

	ctx->sb = sb;
	ctx->driver = driver;
	mountp->fs_data = ctx;

	LOG_INF("VFS adapter mounted successfully");
	return 0;
}

static int vfs_adapter_unmount(struct fs_mount_t *mountp)
{
	struct vfs_adapter_ctx *ctx;
	int ret;

	LOG_INF("VFS adapter unmount");

	if (mountp == NULL || mountp->fs_data == NULL) {
		return -EINVAL;
	}

	ctx = (struct vfs_adapter_ctx *)mountp->fs_data;

	/* Call driver unmount */
	ret = ctx->driver->ops->unmount(ctx->sb);

	/* Free resources */
	vfs_sb_free(ctx->sb);
	k_free(ctx);
	mountp->fs_data = NULL;

	return ret;
}

static int vfs_adapter_unlink(struct fs_mount_t *mountp, const char *name)
{
	struct vfs_adapter_ctx *ctx;
	struct vfs_inode *parent = NULL;
	char *filename = NULL;
	int ret;

	if (mountp == NULL || mountp->fs_data == NULL || name == NULL) {
		return -EINVAL;
	}

	ctx = (struct vfs_adapter_ctx *)mountp->fs_data;

	/* Strip mount point prefix from path */
	const char *rel_path = fs_impl_strip_prefix(name, mountp);

	/* Lookup parent directory */
	ret = vfs_adapter_lookup_path(ctx->sb, rel_path, &parent, NULL, &filename);
	if (ret < 0 && ret != -ENOENT) {
		return ret;
	}

	if (parent == NULL || filename == NULL) {
		ret = -EINVAL;
		goto cleanup;
	}

	/* Call driver unlink */
	ret = ctx->driver->ops->unlink(parent, filename);

cleanup:
	if (parent) {
		vfs_inode_unref(parent);
	}
	if (filename) {
		k_free(filename);
	}
	return ret;
}

static int vfs_adapter_rename(struct fs_mount_t *mountp, const char *from, const char *to)
{
	/* Simplified implementation - full implementation would parse both paths */
	return -ENOTSUP;
}

static int vfs_adapter_mkdir(struct fs_mount_t *mountp, const char *name)
{
	struct vfs_adapter_ctx *ctx;
	struct vfs_inode *parent = NULL, *new_dir = NULL;
	char *dirname = NULL;
	int ret;

	if (mountp == NULL || mountp->fs_data == NULL || name == NULL) {
		return -EINVAL;
	}

	ctx = (struct vfs_adapter_ctx *)mountp->fs_data;

	/* Strip mount point prefix from path */
	const char *rel_path = fs_impl_strip_prefix(name, mountp);

	/* Lookup parent directory */
	ret = vfs_adapter_lookup_path(ctx->sb, rel_path, &parent, NULL, &dirname);
	if (ret < 0 && ret != -ENOENT) {
		return ret;
	}

	if (parent == NULL || dirname == NULL) {
		ret = -EINVAL;
		goto cleanup;
	}

	/* Call driver mkdir */
	ret = ctx->driver->ops->mkdir(parent, dirname, 0755, &new_dir);
	if (ret == 0 && new_dir) {
		vfs_inode_unref(new_dir);
	}

cleanup:
	if (parent) {
		vfs_inode_unref(parent);
	}
	if (dirname) {
		k_free(dirname);
	}
	return ret;
}

static int vfs_adapter_opendir(struct fs_dir_t *dirp, const char *fs_path)
{
	struct vfs_adapter_ctx *ctx;
	struct vfs_inode *inode = NULL;
	int ret;

	if (dirp == NULL || dirp->mp == NULL || dirp->mp->fs_data == NULL) {
		return -EINVAL;
	}

	ctx = (struct vfs_adapter_ctx *)dirp->mp->fs_data;

	/* Strip mount point prefix from path */
	const char *rel_path = fs_impl_strip_prefix(fs_path, dirp->mp);

	/* Lookup directory inode */
	ret = vfs_adapter_lookup_path(ctx->sb, rel_path, NULL, &inode, NULL);
	if (ret < 0) {
		return ret;
	}

	/* Call driver opendir */
	ret = ctx->driver->ops->opendir(inode, dirp);
	if (ret < 0) {
		vfs_inode_unref(inode);
	}

	return ret;
}

static int vfs_adapter_readdir(struct fs_dir_t *dirp, struct fs_dirent *entry)
{
	struct vfs_adapter_ctx *ctx;

	if (dirp == NULL || dirp->mp == NULL || dirp->mp->fs_data == NULL || entry == NULL) {
		return -EINVAL;
	}

	ctx = (struct vfs_adapter_ctx *)dirp->mp->fs_data;

	/* Call driver readdir */
	return ctx->driver->ops->readdir(dirp, entry);
}

static int vfs_adapter_closedir(struct fs_dir_t *dirp)
{
	struct vfs_adapter_ctx *ctx;

	if (dirp == NULL || dirp->mp == NULL || dirp->mp->fs_data == NULL) {
		return -EINVAL;
	}

	ctx = (struct vfs_adapter_ctx *)dirp->mp->fs_data;

	/* Call driver closedir */
	return ctx->driver->ops->closedir(dirp);
}

static int vfs_adapter_open(struct fs_file_t *filp, const char *fs_path, fs_mode_t flags)
{
	struct vfs_adapter_ctx *ctx;
	struct vfs_inode *parent = NULL, *inode = NULL;
	char *filename = NULL;
	int ret;
	bool create = (flags & FS_O_CREATE) != 0;

	if (filp == NULL || filp->mp == NULL || filp->mp->fs_data == NULL) {
		return -EINVAL;
	}

	ctx = (struct vfs_adapter_ctx *)filp->mp->fs_data;

	/* Strip mount point prefix from path */
	const char *rel_path = fs_impl_strip_prefix(fs_path, filp->mp);

	/* Lookup file inode */
	ret = vfs_adapter_lookup_path(ctx->sb, rel_path, &parent, &inode, &filename);

	if (ret == -ENOENT && create && parent != NULL && filename != NULL) {
		/* Create new file */
		ret = ctx->driver->ops->create(parent, filename, 0644, &inode);
		if (ret < 0) {
			goto cleanup;
		}
	} else if (ret < 0) {
		goto cleanup;
	} else if (inode != NULL && (flags & FS_O_TRUNC)) {
		/* Truncate existing file when FS_O_TRUNC is set */
		if (ctx->driver->ops->truncate) {
			ctx->driver->ops->truncate(inode, 0);
		}
	}

	/* Call driver open */
	ret = ctx->driver->ops->open(inode, filp);
	if (ret < 0) {
		vfs_inode_unref(inode);
	}

cleanup:
	if (parent) {
		vfs_inode_unref(parent);
	}
	if (filename) {
		k_free(filename);
	}
	return ret;
}

static int vfs_adapter_close(struct fs_file_t *filp)
{
	struct vfs_adapter_ctx *ctx;

	if (filp == NULL || filp->mp == NULL || filp->mp->fs_data == NULL) {
		return -EINVAL;
	}

	ctx = (struct vfs_adapter_ctx *)filp->mp->fs_data;

	/* Call driver close */
	return ctx->driver->ops->close(filp);
}

static ssize_t vfs_adapter_read(struct fs_file_t *filp, void *dest, size_t nbytes)
{
	struct vfs_adapter_ctx *ctx;

	if (filp == NULL || filp->mp == NULL || filp->mp->fs_data == NULL) {
		return -EINVAL;
	}

	ctx = (struct vfs_adapter_ctx *)filp->mp->fs_data;

	/* Call driver read */
	return ctx->driver->ops->read(filp, dest, nbytes);
}

static ssize_t vfs_adapter_write(struct fs_file_t *filp, const void *src, size_t nbytes)
{
	struct vfs_adapter_ctx *ctx;

	if (filp == NULL || filp->mp == NULL || filp->mp->fs_data == NULL) {
		return -EINVAL;
	}

	ctx = (struct vfs_adapter_ctx *)filp->mp->fs_data;

	/* Call driver write */
	return ctx->driver->ops->write(filp, src, nbytes);
}

static int vfs_adapter_lseek(struct fs_file_t *filp, off_t off, int whence)
{
	struct vfs_adapter_ctx *ctx;

	if (filp == NULL || filp->mp == NULL || filp->mp->fs_data == NULL) {
		return -EINVAL;
	}

	ctx = (struct vfs_adapter_ctx *)filp->mp->fs_data;

	/* Call driver lseek */
	return ctx->driver->ops->lseek(filp, off, whence);
}

static off_t vfs_adapter_tell(struct fs_file_t *filp)
{
	struct ramfs_file_data *file_data;

	if (filp == NULL || filp->filep == NULL) {
		return -EINVAL;
	}

	file_data = (struct ramfs_file_data *)filp->filep;
	return file_data->pos;
}

static int vfs_adapter_truncate(struct fs_file_t *filp, off_t length)
{
	struct vfs_adapter_ctx *ctx;
	struct ramfs_file_data *file_data;

	if (filp == NULL || filp->mp == NULL || filp->mp->fs_data == NULL || filp->filep == NULL) {
		return -EINVAL;
	}

	ctx = (struct vfs_adapter_ctx *)filp->mp->fs_data;
	file_data = (struct ramfs_file_data *)filp->filep;

	/* Call driver truncate */
	return ctx->driver->ops->truncate(file_data->inode, length);
}

static int vfs_adapter_sync(struct fs_file_t *filp)
{
	struct vfs_adapter_ctx *ctx;

	if (filp == NULL || filp->mp == NULL || filp->mp->fs_data == NULL) {
		return -EINVAL;
	}

	ctx = (struct vfs_adapter_ctx *)filp->mp->fs_data;

	/* Call driver sync if available */
	if (ctx->driver->ops->sync) {
		return ctx->driver->ops->sync(filp);
	}

	return 0;
}

static int vfs_adapter_stat(struct fs_mount_t *mountp, const char *path, struct fs_dirent *entry)
{
	struct vfs_adapter_ctx *ctx;
	struct vfs_inode *inode = NULL;
	int ret;

	if (mountp == NULL || mountp->fs_data == NULL || path == NULL || entry == NULL) {
		return -EINVAL;
	}

	ctx = (struct vfs_adapter_ctx *)mountp->fs_data;

	/* Strip mount point prefix from path */
	const char *rel_path = fs_impl_strip_prefix(path, mountp);

	/* Lookup inode */
	ret = vfs_adapter_lookup_path(ctx->sb, rel_path, NULL, &inode, NULL);
	if (ret < 0) {
		return ret;
	}

	/* Call driver stat */
	ret = ctx->driver->ops->stat(inode, entry);
	vfs_inode_unref(inode);

	return ret;
}

static int vfs_adapter_statvfs(struct fs_mount_t *mountp, const char *path,
			       struct fs_statvfs *stat)
{
	struct vfs_adapter_ctx *ctx;

	if (mountp == NULL || mountp->fs_data == NULL || stat == NULL) {
		return -EINVAL;
	}

	ctx = (struct vfs_adapter_ctx *)mountp->fs_data;

	/* Call driver statvfs */
	return ctx->driver->ops->statvfs(ctx->sb, stat);
}

/* Zephyr filesystem operations structure */
static struct fs_file_system_t vfs_adapter_fs = {
	.mount = vfs_adapter_mount,
	.unmount = vfs_adapter_unmount,
	.unlink = vfs_adapter_unlink,
	.rename = vfs_adapter_rename,
	.mkdir = vfs_adapter_mkdir,
	.opendir = vfs_adapter_opendir,
	.readdir = vfs_adapter_readdir,
	.closedir = vfs_adapter_closedir,
	.open = vfs_adapter_open,
	.close = vfs_adapter_close,
	.read = vfs_adapter_read,
	.write = vfs_adapter_write,
	.lseek = vfs_adapter_lseek,
	.tell = vfs_adapter_tell,
	.truncate = vfs_adapter_truncate,
	.sync = vfs_adapter_sync,
	.stat = vfs_adapter_stat,
	.statvfs = vfs_adapter_statvfs,
};

struct fs_file_system_t *vfs_adapter_register(const struct vfs_driver *driver)
{
	int ret;

	if (driver == NULL) {
		return NULL;
	}

	/* Register with Zephyr */
	ret = fs_register(driver->type, &vfs_adapter_fs);
	if (ret < 0) {
		LOG_ERR("Failed to register VFS adapter: %d", ret);
		return NULL;
	}

	LOG_INF("VFS adapter registered for driver: %s", driver->name);
	return &vfs_adapter_fs;
}

int vfs_adapter_unregister(struct fs_file_system_t *fs)
{
	int ret;

	if (fs == NULL) {
		return -EINVAL;
	}

	ret = fs_unregister(FS_TYPE_EXTERNAL_BASE, fs);
	if (ret < 0) {
		LOG_ERR("Failed to unregister VFS adapter: %d", ret);
		return ret;
	}

	return 0;
}
