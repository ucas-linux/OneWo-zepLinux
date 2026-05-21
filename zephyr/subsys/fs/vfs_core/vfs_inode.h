/*
 * Copyright (c) 2024 OneWo-zepLinux Project
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_SUBSYS_FS_VFS_CORE_VFS_INODE_H_
#define ZEPHYR_SUBSYS_FS_VFS_CORE_VFS_INODE_H_

#include <zephyr/kernel.h>
#include <zephyr/sys/dlist.h>
#include <zephyr/fs/fs.h>
#include <sys/types.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct vfs_superblock;
struct vfs_inode;

/**
 * @brief VFS inode structure
 *
 * Represents a file system object (file, directory, etc.)
 */
struct vfs_inode {
	ino_t ino;                      /* Inode number */
	struct vfs_superblock *sb;      /* Pointer to superblock */
	mode_t mode;                    /* File type and permissions */
	uid_t uid;                      /* Owner user ID */
	gid_t gid;                      /* Owner group ID */
	off_t size;                     /* File size in bytes */
	struct timespec mtime;          /* Modification time */
	struct timespec atime;          /* Access time */
	struct timespec ctime;          /* Change time */
	int ref_count;                  /* Reference counter */
	void *fs_data;                  /* FS-specific private data */
	sys_dnode_t node;               /* Link in inode list */
};

/**
 * @brief Allocate a new inode
 *
 * @param sb Superblock to which the inode belongs
 * @return Pointer to allocated inode, or NULL on failure
 */
struct vfs_inode *vfs_inode_alloc(struct vfs_superblock *sb);

/**
 * @brief Free an inode
 *
 * @param inode Inode to free
 */
void vfs_inode_free(struct vfs_inode *inode);

/**
 * @brief Increment inode reference count
 *
 * @param inode Inode to reference
 * @return Updated reference count
 */
int vfs_inode_ref(struct vfs_inode *inode);

/**
 * @brief Decrement inode reference count
 *
 * @param inode Inode to unreference
 * @return Updated reference count, or negative error code
 */
int vfs_inode_unref(struct vfs_inode *inode);

/**
 * @brief Initialize inode with metadata
 *
 * @param inode Inode to initialize
 * @param mode File mode (type and permissions)
 * @param uid Owner user ID
 * @param gid Owner group ID
 * @return 0 on success, negative errno on failure
 */
int vfs_inode_init(struct vfs_inode *inode, mode_t mode, uid_t uid, gid_t gid);

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_SUBSYS_FS_VFS_CORE_VFS_INODE_H_ */
