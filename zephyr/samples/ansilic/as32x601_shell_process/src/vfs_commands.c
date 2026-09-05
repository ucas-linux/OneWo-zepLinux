/*
 * VFS RAMFS commands for as32x601_shell_process
 * Mounts ramfs at /tmp and provides: ls <path>, mkdir, cat
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/fs/fs.h>
#include <string.h>
#include <errno.h>
#include "shell_process.h"

/* ramfs_init() is defined in zephyr/subsys/fs/ramfs/ramfs.h */
extern int ramfs_init(void);

#define VFS_TMP_MOUNT "/tmp"

static struct fs_mount_t tmp_mnt = {
	.type        = FS_TYPE_EXTERNAL_BASE,
	.mnt_point   = VFS_TMP_MOUNT,
	.storage_dev = (void *)"ramfs",
	.flags       = 0,
};

static int vfs_mount_tmp(void)
{
	int ret;

	/* Initialize and register ramfs driver (vfs_subsystem_init may have done this already) */
	ret = ramfs_init();
	if (ret != 0 && ret != -EEXIST) {
		printk("VFS: failed to initialize ramfs driver: %d\n", ret);
		return 0; /* non-fatal: shell still boots */
	}

	/* Now mount ramfs at /tmp */
	printk("VFS: Mounting ramfs at /tmp...\n");
	ret = fs_mount(&tmp_mnt);
	if (ret != 0) {
		printk("VFS: failed to mount ramfs at /tmp: %d\n", ret);
	} else {
		printk("VFS: ramfs mounted at /tmp\n");
	}
	return 0; /* non-fatal: shell still boots */
}

SYS_INIT(vfs_mount_tmp, APPLICATION, 100);

/* ------------------------------------------------------------------ */
/* ls [path]  — list a VFS directory                                   */
/* ------------------------------------------------------------------ */
int cmd_ls_vfs(int argc, char **argv)
{
	const char *path = (argc > 1) ? argv[1] : VFS_TMP_MOUNT;
	struct fs_dir_t dir;
	struct fs_dirent ent;
	int ret;

	fs_dir_t_init(&dir);
	ret = fs_opendir(&dir, path);
	if (ret != 0) {
		printk("ls: cannot open '%s': %d\n", path, ret);
		return ret;
	}

	while (true) {
		ret = fs_readdir(&dir, &ent);
		if (ret != 0 || ent.name[0] == '\0') {
			break;
		}
		if (ent.type == FS_DIR_ENTRY_DIR) {
			printk("%s/\n", ent.name);
		} else {
			printk("%-24s %zu bytes\n", ent.name, ent.size);
		}
	}

	fs_closedir(&dir);
	return 0;
}

/* ------------------------------------------------------------------ */
/* mkdir <path>                                                        */
/* ------------------------------------------------------------------ */
static int cmd_mkdir_vfs(int argc, char **argv)
{
	if (argc < 2) {
		printk("Usage: mkdir <path>\n");
		return -EINVAL;
	}

	int ret = fs_mkdir(argv[1]);

	if (ret != 0) {
		printk("mkdir: '%s': %d\n", argv[1], ret);
	}
	return ret;
}

/* ------------------------------------------------------------------ */
/* cat <file>                                                          */
/* ------------------------------------------------------------------ */
static int cmd_cat_vfs(int argc, char **argv)
{
	if (argc < 2) {
		printk("Usage: cat <file>\n");
		return -EINVAL;
	}

	struct fs_file_t f;
	char buf[128];
	ssize_t n;

	fs_file_t_init(&f);
	int ret = fs_open(&f, argv[1], FS_O_READ);

	if (ret != 0) {
		printk("cat: cannot open '%s': %d\n", argv[1], ret);
		return ret;
	}

	while ((n = fs_read(&f, buf, sizeof(buf) - 1)) > 0) {
		buf[n] = '\0';
		printk("%s", buf);
	}
	/* ensure trailing newline if file doesn't end with one */
	if (n == 0 && buf[0] != '\0' && buf[strlen(buf) - 1] != '\n') {
		printk("\n");
	}

	fs_close(&f);
	return 0;
}

/* ------------------------------------------------------------------ */
/* mount_status - debug command to check mount status                 */
/* ------------------------------------------------------------------ */
static int cmd_mount_status(int argc, char **argv)
{
	printk("Mounted filesystems:\n");

	/* Try to access /tmp to see if it's mounted */
	struct fs_dir_t dir;
	fs_dir_t_init(&dir);
	int ret = fs_opendir(&dir, "/tmp");
	if (ret == 0) {
		printk("  /tmp - accessible (mounted)\n");
		fs_closedir(&dir);
	} else {
		printk("  /tmp - not accessible: %d\n", ret);
	}

	return 0;
}

/* ------------------------------------------------------------------ */
/* remount_tmp - debug command to retry mounting ramfs                */
/* ------------------------------------------------------------------ */
static int cmd_remount_tmp(int argc, char **argv)
{
	int ret;

	printk("Re-initializing ramfs driver...\n");
	ret = ramfs_init();
	if (ret == -EEXIST) {
		ret = 0; /* already registered, that's fine */
	}
	printk("ramfs_init() returned: %d\n", ret);

	printk("Attempting to mount ramfs at /tmp...\n");
	ret = fs_mount(&tmp_mnt);
	printk("fs_mount() returned: %d\n", ret);

	if (ret == 0) {
		printk("Successfully mounted!\n");
	}

	return ret;
}

/* ------------------------------------------------------------------ */
/* Register commands                                                   */
/* ------------------------------------------------------------------ */
static const struct shell_cmd vfs_cmd_mkdir = {
	.name  = "mkdir",
	.exec  = cmd_mkdir_vfs,
	.brief = "Create VFS directory",
};

static const struct shell_cmd vfs_cmd_cat = {
	.name  = "cat",
	.exec  = cmd_cat_vfs,
	.brief = "Print VFS file",
};

static const struct shell_cmd vfs_cmd_mount_status = {
	.name  = "mount_status",
	.exec  = cmd_mount_status,
	.brief = "Show mount status",
};

static const struct shell_cmd vfs_cmd_remount = {
	.name  = "remount_tmp",
	.exec  = cmd_remount_tmp,
	.brief = "Retry mounting /tmp",
};

static int register_vfs_commands(void)
{
	shell_cmd_register(&vfs_cmd_mkdir);
	shell_cmd_register(&vfs_cmd_cat);
	shell_cmd_register(&vfs_cmd_mount_status);
	shell_cmd_register(&vfs_cmd_remount);
	return 0;
}

SYS_INIT(register_vfs_commands, APPLICATION, 101);
