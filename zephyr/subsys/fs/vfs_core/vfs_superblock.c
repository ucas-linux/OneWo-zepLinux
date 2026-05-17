/*
 * Copyright (c) 2024 OneWo-zepLinux Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <errno.h>
#include <string.h>
#include "vfs_superblock.h"
#include "vfs_inode.h"

#define LOG_LEVEL CONFIG_FS_LOG_LEVEL
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(vfs_superblock);

struct vfs_superblock *vfs_sb_alloc(const struct vfs_driver *driver, const char *mnt_point)
{
	struct vfs_superblock *sb;

	if (driver == NULL || mnt_point == NULL) {
		LOG_ERR("Invalid parameters");
		return NULL;
	}

	sb = k_malloc(sizeof(struct vfs_superblock));
	if (sb == NULL) {
		LOG_ERR("Failed to allocate superblock");
		return NULL;
	}

	memset(sb, 0, sizeof(struct vfs_superblock));
	sb->driver = driver;
	sb->mnt_point = k_malloc(strlen(mnt_point) + 1);
	if (sb->mnt_point == NULL) {
		LOG_ERR("Failed to allocate mount point string");
		k_free(sb);
		return NULL;
	}
	strcpy(sb->mnt_point, mnt_point);

	k_mutex_init(&sb->lock);
	sb->usage_count = 0;

	LOG_DBG("Allocated superblock for %s", mnt_point);
	return sb;
}

void vfs_sb_free(struct vfs_superblock *sb)
{
	if (sb == NULL) {
		return;
	}

	if (sb->usage_count > 0) {
		LOG_WRN("Freeing superblock with usage_count=%d", sb->usage_count);
	}

	if (sb->mnt_point != NULL) {
		k_free(sb->mnt_point);
	}

	LOG_DBG("Freeing superblock");
	k_free(sb);
}

int vfs_sb_lock(struct vfs_superblock *sb)
{
	if (sb == NULL) {
		return -EINVAL;
	}

	return k_mutex_lock(&sb->lock, K_FOREVER);
}

int vfs_sb_unlock(struct vfs_superblock *sb)
{
	if (sb == NULL) {
		return -EINVAL;
	}

	return k_mutex_unlock(&sb->lock);
}
