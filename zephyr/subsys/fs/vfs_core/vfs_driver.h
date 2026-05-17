/*
 * Copyright (c) 2024 OneWo-zepLinux Project
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_SUBSYS_FS_VFS_CORE_VFS_DRIVER_H_
#define ZEPHYR_SUBSYS_FS_VFS_CORE_VFS_DRIVER_H_

#include <zephyr/kernel.h>
#include <zephyr/fs/fs.h>
#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
struct vfs_superblock;
struct vfs_inode;

/**
 * @brief VFS driver operations table
 *
 * Defines the operations that a file system driver must implement
 */
struct vfs_driver_ops {
	/* Mount/unmount operations */
	int (*mount)(struct vfs_superblock *sb, const char *source, uint32_t flags);
	int (*unmount)(struct vfs_superblock *sb);

	/* Inode operations */
	int (*lookup)(struct vfs_inode *dir, const char *name, struct vfs_inode **result);
	int (*create)(struct vfs_inode *dir, const char *name, mode_t mode,
		      struct vfs_inode **result);
	int (*mkdir)(struct vfs_inode *dir, const char *name, mode_t mode,
		     struct vfs_inode **result);
	int (*unlink)(struct vfs_inode *dir, const char *name);
	int (*rmdir)(struct vfs_inode *dir, const char *name);
	int (*rename)(struct vfs_inode *old_dir, const char *old_name,
		      struct vfs_inode *new_dir, const char *new_name);

	/* File operations */
	int (*open)(struct vfs_inode *inode, struct fs_file_t *file);
	int (*close)(struct fs_file_t *file);
	ssize_t (*read)(struct fs_file_t *file, void *buf, size_t size);
	ssize_t (*write)(struct fs_file_t *file, const void *buf, size_t size);
	int (*lseek)(struct fs_file_t *file, off_t offset, int whence);
	int (*truncate)(struct vfs_inode *inode, off_t length);
	int (*sync)(struct fs_file_t *file);

	/* Directory operations */
	int (*opendir)(struct vfs_inode *inode, struct fs_dir_t *dir);
	int (*readdir)(struct fs_dir_t *dir, struct fs_dirent *entry);
	int (*closedir)(struct fs_dir_t *dir);

	/* Stat operations */
	int (*stat)(struct vfs_inode *inode, struct fs_dirent *entry);
	int (*statvfs)(struct vfs_superblock *sb, struct fs_statvfs *stat);
};

/**
 * @brief VFS driver structure
 *
 * Represents a file system driver
 */
struct vfs_driver {
	const char *name;               /* Driver name */
	int type;                       /* FS type identifier */
	const struct vfs_driver_ops *ops; /* Driver operations */
};

/**
 * @brief Register a VFS driver
 *
 * @param driver Driver to register
 * @return 0 on success, negative errno on failure
 */
int vfs_driver_register(const struct vfs_driver *driver);

/**
 * @brief Unregister a VFS driver
 *
 * @param driver Driver to unregister
 * @return 0 on success, negative errno on failure
 */
int vfs_driver_unregister(const struct vfs_driver *driver);

/**
 * @brief Find a VFS driver by name
 *
 * @param name Driver name
 * @return Pointer to driver, or NULL if not found
 */
const struct vfs_driver *vfs_driver_find(const char *name);

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_SUBSYS_FS_VFS_CORE_VFS_DRIVER_H_ */
