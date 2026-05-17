/*
 * Copyright (c) 2024 OneWo-zepLinux Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <errno.h>
#include <string.h>
#include "vfs_driver.h"

#define LOG_LEVEL CONFIG_FS_LOG_LEVEL
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(vfs_driver);

#define MAX_VFS_DRIVERS 8

/* Global driver registry */
static const struct vfs_driver *vfs_drivers[MAX_VFS_DRIVERS];
static struct k_mutex driver_lock;
static bool driver_lock_initialized;

static void ensure_driver_lock_init(void)
{
	if (!driver_lock_initialized) {
		k_mutex_init(&driver_lock);
		driver_lock_initialized = true;
	}
}

int vfs_driver_register(const struct vfs_driver *driver)
{
	int i;

	if (driver == NULL || driver->name == NULL || driver->ops == NULL) {
		LOG_ERR("Invalid driver");
		return -EINVAL;
	}

	ensure_driver_lock_init();
	k_mutex_lock(&driver_lock, K_FOREVER);

	/* Check if driver already registered */
	for (i = 0; i < MAX_VFS_DRIVERS; i++) {
		if (vfs_drivers[i] == driver) {
			LOG_WRN("Driver %s already registered", driver->name);
			k_mutex_unlock(&driver_lock);
			return -EEXIST;
		}
	}

	/* Find empty slot */
	for (i = 0; i < MAX_VFS_DRIVERS; i++) {
		if (vfs_drivers[i] == NULL) {
			vfs_drivers[i] = driver;
			LOG_INF("Registered VFS driver: %s", driver->name);
			k_mutex_unlock(&driver_lock);
			return 0;
		}
	}

	LOG_ERR("No space for driver %s", driver->name);
	k_mutex_unlock(&driver_lock);
	return -ENOMEM;
}

int vfs_driver_unregister(const struct vfs_driver *driver)
{
	int i;

	if (driver == NULL) {
		return -EINVAL;
	}

	ensure_driver_lock_init();
	k_mutex_lock(&driver_lock, K_FOREVER);

	for (i = 0; i < MAX_VFS_DRIVERS; i++) {
		if (vfs_drivers[i] == driver) {
			vfs_drivers[i] = NULL;
			LOG_INF("Unregistered VFS driver: %s", driver->name);
			k_mutex_unlock(&driver_lock);
			return 0;
		}
	}

	LOG_WRN("Driver %s not found", driver->name);
	k_mutex_unlock(&driver_lock);
	return -ENOENT;
}

const struct vfs_driver *vfs_driver_find(const char *name)
{
	int i;
	const struct vfs_driver *driver = NULL;

	if (name == NULL) {
		return NULL;
	}

	ensure_driver_lock_init();
	k_mutex_lock(&driver_lock, K_FOREVER);

	for (i = 0; i < MAX_VFS_DRIVERS; i++) {
		if (vfs_drivers[i] != NULL &&
		    strcmp(vfs_drivers[i]->name, name) == 0) {
			driver = vfs_drivers[i];
			break;
		}
	}

	k_mutex_unlock(&driver_lock);

	if (driver == NULL) {
		LOG_DBG("Driver %s not found", name);
	}

	return driver;
}
