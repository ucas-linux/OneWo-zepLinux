/*
 * Copyright (c) 2024 OneWo-zepLinux Project
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ZEPHYR_SUBSYS_FS_VFS_CORE_VFS_ADAPTER_H_
#define ZEPHYR_SUBSYS_FS_VFS_CORE_VFS_ADAPTER_H_

#include <zephyr/fs/fs.h>
#include "vfs_driver.h"
#include "vfs_superblock.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Register VFS adapter with Zephyr filesystem
 *
 * This creates a Zephyr fs_file_system_t that wraps the VFS driver
 *
 * @param driver VFS driver to wrap
 * @return Pointer to Zephyr filesystem structure, or NULL on failure
 */
struct fs_file_system_t *vfs_adapter_register(const struct vfs_driver *driver);

/**
 * @brief Unregister VFS adapter
 *
 * @param fs Zephyr filesystem structure to unregister
 * @return 0 on success, negative errno on failure
 */
int vfs_adapter_unregister(struct fs_file_system_t *fs);

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_SUBSYS_FS_VFS_CORE_VFS_ADAPTER_H_ */
