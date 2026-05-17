/*
 * Copyright (c) 2024 OneWo-zepLinux Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/sys/util.h>
#include <errno.h>
#include <string.h>
#include "vfs_inode.h"
#include "vfs_superblock.h"

#define LOG_LEVEL CONFIG_FS_LOG_LEVEL
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(vfs_inode);

/* Global inode counter for generating unique inode numbers */
static atomic_t inode_counter = ATOMIC_INIT(1);

struct vfs_inode *vfs_inode_alloc(struct vfs_superblock *sb)
{
	struct vfs_inode *inode;

	if (sb == NULL) {
		LOG_ERR("Invalid superblock");
		return NULL;
	}

	inode = k_malloc(sizeof(struct vfs_inode));
	if (inode == NULL) {
		LOG_ERR("Failed to allocate inode");
		return NULL;
	}

	memset(inode, 0, sizeof(struct vfs_inode));
	inode->ino = atomic_inc(&inode_counter);
	inode->sb = sb;
	inode->ref_count = 1;

	LOG_DBG("Allocated inode %lu", (unsigned long)inode->ino);
	return inode;
}

void vfs_inode_free(struct vfs_inode *inode)
{
	if (inode == NULL) {
		return;
	}

	if (inode->ref_count > 0) {
		LOG_WRN("Freeing inode %lu with ref_count=%d",
			(unsigned long)inode->ino, inode->ref_count);
	}

	LOG_DBG("Freeing inode %lu", (unsigned long)inode->ino);
	k_free(inode);
}

int vfs_inode_ref(struct vfs_inode *inode)
{
	if (inode == NULL) {
		return -EINVAL;
	}

	inode->ref_count++;
	LOG_DBG("Inode %lu ref_count=%d", (unsigned long)inode->ino, inode->ref_count);
	return inode->ref_count;
}

int vfs_inode_unref(struct vfs_inode *inode)
{
	if (inode == NULL) {
		return -EINVAL;
	}

	if (inode->ref_count <= 0) {
		LOG_ERR("Inode %lu ref_count already 0", (unsigned long)inode->ino);
		return -EINVAL;
	}

	inode->ref_count--;
	LOG_DBG("Inode %lu ref_count=%d", (unsigned long)inode->ino, inode->ref_count);

	if (inode->ref_count == 0) {
		vfs_inode_free(inode);
		return 0;
	}

	return inode->ref_count;
}

int vfs_inode_init(struct vfs_inode *inode, mode_t mode, uid_t uid, gid_t gid)
{
	if (inode == NULL) {
		return -EINVAL;
	}

	inode->mode = mode;
	inode->uid = uid;
	inode->gid = gid;
	inode->size = 0;

	/* Set current time for all timestamps */
	k_timepoint_t now = sys_timepoint_calc(K_SECONDS(0));
	uint64_t now_ns = sys_timepoint_timeout(now).ticks * 1000000000ULL / CONFIG_SYS_CLOCK_TICKS_PER_SEC;

	inode->mtime.tv_sec = now_ns / 1000000000ULL;
	inode->mtime.tv_nsec = now_ns % 1000000000ULL;
	inode->atime = inode->mtime;
	inode->ctime = inode->mtime;

	LOG_DBG("Initialized inode %lu mode=0%o", (unsigned long)inode->ino, mode);
	return 0;
}
