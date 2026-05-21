# SPDX-License-Identifier: Apache-2.0

if(CONFIG_VFS_CORE)
  zephyr_library()
  zephyr_library_include_directories(${CMAKE_CURRENT_SOURCE_DIR})
  zephyr_library_sources(vfs_inode.c)
  zephyr_library_sources(vfs_superblock.c)
  zephyr_library_sources(vfs_driver.c)
  zephyr_library_sources(vfs_adapter.c)
  zephyr_library_sources(vfs_init.c)
endif()
