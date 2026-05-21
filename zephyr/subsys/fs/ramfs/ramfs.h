/*
 * Copyright (c) 2024 OneWo-zepLinux Project
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_SUBSYS_FS_RAMFS_RAMFS_H_
#define ZEPHYR_SUBSYS_FS_RAMFS_RAMFS_H_

#include <zephyr/kernel.h>
#include <zephyr/sys/dlist.h>
#include "../vfs_core/vfs_driver.h"
#include "../vfs_core/vfs_inode.h"

#ifdef __cplusplus
extern "C" {
#endif

/* RAMFS-specific inode data */
struct ramfs_inode_data {
	sys_dlist_t children;           /* List of directory entries (for directories) */
	void *data;                     /* File data buffer (for files) */
	size_t data_capacity;           /* Allocated capacity for data buffer */
};

/* RAMFS directory entry */
struct ramfs_dirent {
	sys_dnode_t node;               /* Link in parent's children list */
	char *name;                     /* Entry name */
	struct vfs_inode *inode;        /* Inode for this entry */
};

/* RAMFS superblock data */
struct ramfs_sb_data {
	size_t total_size;              /* Total allocated size */
	size_t max_size;                /* Maximum allowed size */
};

/* RAMFS file handle data */
struct ramfs_file_data {
	struct vfs_inode *inode;        /* Associated inode */
	off_t pos;                      /* Current file position */
};

/* RAMFS directory handle data */
struct ramfs_dir_data {
	struct vfs_inode *inode;        /* Associated inode */
	sys_dnode_t *current;           /* Current position in directory */
	int entry_count;                /* Number of entries read */
};

/**
 * @brief Initialize RAMFS driver
 *
 * @return 0 on success, negative errno on failure
 */
int ramfs_init(void);

/**
 * @brief Get the RAMFS driver instance
 *
 * @return Pointer to RAMFS driver
 */
const struct vfs_driver *ramfs_get_driver(void);

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_SUBSYS_FS_RAMFS_RAMFS_H_ */
