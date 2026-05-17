/*
 * Copyright (c) 2024 OneWo-zepLinux Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/init.h>
#include "../ramfs/ramfs.h"
#include "vfs_adapter.h"

#define LOG_LEVEL CONFIG_FS_LOG_LEVEL
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(vfs_init);

static int vfs_subsystem_init(void)
{
	int ret;

	LOG_INF("Initializing VFS subsystem");

#ifdef CONFIG_RAMFS
	/* Initialize RAMFS driver */
	ret = ramfs_init();
	if (ret < 0) {
		LOG_ERR("Failed to initialize RAMFS: %d", ret);
		return ret;
	}

	/* Register RAMFS with Zephyr filesystem */
	if (vfs_adapter_register(ramfs_get_driver()) == NULL) {
		LOG_ERR("Failed to register RAMFS adapter");
		return -EINVAL;
	}

	LOG_INF("RAMFS registered successfully");
#endif

	LOG_INF("VFS subsystem initialized");
	return 0;
}

SYS_INIT(vfs_subsystem_init, APPLICATION, CONFIG_FILE_SYSTEM_INIT_PRIORITY);
