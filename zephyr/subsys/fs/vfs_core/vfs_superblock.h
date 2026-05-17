/*
 * Copyright (c) 2024 OneWo-zepLinux Project
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_SUBSYS_FS_VFS_CORE_VFS_SUPERBLOCK_H_
#define ZEPHYR_SUBSYS_FS_VFS_CORE_VFS_SUPERBLOCK_H_

#include <zephyr/kernel.h>
#include <zephyr/sys/dlist.h>
#include "vfs_driver.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct vfs_inode;

/**
 * @brief VFS superblock structure
 *
 * Represents a mounted file system instance
 */
struct vfs_superblock {
	const struct vfs_driver *driver;    /* FS driver */
	sys_dnode_t node;                   /* Link in mount list */
	struct vfs_inode *root_inode;       /* Root directory inode */
	void *fs_data;                      /* FS-specific data */
	uint32_t flags;                     /* Mount flags */
	struct k_mutex lock;                /* Superblock lock */
	int usage_count;                    /* Active operations count */
	char *mnt_point;                    /* Mount point path */
};

/**
 * @brief Allocate a new superblock
 *
 * @param driver File system driver
 * @param mnt_point Mount point path
 * @return Pointer to allocated superblock, or NULL on failure
 */
struct vfs_superblock *vfs_sb_alloc(const struct vfs_driver *driver, const char *mnt_point);

/**
 * @brief Free a superblock
 *
 * @param sb Superblock to free
 */
void vfs_sb_free(struct vfs_superblock *sb);

/**
 * @brief Lock a superblock
 *
 * @param sb Superblock to lock
 * @return 0 on success, negative errno on failure
 */
int vfs_sb_lock(struct vfs_superblock *sb);

/**
 * @brief Unlock a superblock
 *
 * @param sb Superblock to unlock
 * @return 0 on success, negative errno on failure
 */
int vfs_sb_unlock(struct vfs_superblock *sb);

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_SUBSYS_FS_VFS_CORE_VFS_SUPERBLOCK_H_ */
