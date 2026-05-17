# SPDX-License-Identifier: Apache-2.0

if(CONFIG_RAMFS)
  zephyr_library()
  zephyr_library_include_directories(${CMAKE_CURRENT_SOURCE_DIR})
  zephyr_library_sources(ramfs.c)
endif()
