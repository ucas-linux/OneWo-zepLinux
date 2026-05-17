# VFS RAMFS Test for qemu_cortex_m3

This test validates the VFS (Virtual File System) core layer and RAMFS (RAM File System) implementation on the qemu_cortex_m3 platform.

## Overview

The test suite exercises the following functionality:
- Basic file operations (create, open, read, write, close)
- Directory operations (mkdir, opendir, readdir, closedir)
- File seeking (SEEK_SET, SEEK_CUR, SEEK_END)
- File truncation
- File and directory unlinking
- Stat operations (file and directory metadata)
- Filesystem statistics (statvfs)

## Architecture

The implementation consists of:

1. **VFS Core Layer** (`subsys/fs/vfs_core/`)
   - `vfs_inode.c/h` - Inode management
   - `vfs_superblock.c/h` - Superblock management
   - `vfs_driver.c/h` - Driver interface and registry
   - `vfs_adapter.c/h` - Zephyr filesystem adapter
   - `vfs_init.c` - Subsystem initialization

2. **RAMFS Driver** (`subsys/fs/ramfs/`)
   - `ramfs.c/h` - In-memory filesystem implementation

3. **Test Suite** (`tests/subsys/fs/vfs_ramfs/`)
   - `test_vfs_ramfs.c` - Comprehensive test cases

## Building and Running

### Build for qemu_cortex_m3

```bash
cd /opt/Program/UCAS/OneWo-zepLinux/zephyr
west build -b qemu_cortex_m3 tests/subsys/fs/vfs_ramfs
```

### Run the test

```bash
west build -t run
```

### Expected Output

The test should output:
```
*** Booting Zephyr OS build ... ***
Setting up VFS RAMFS test
RAMFS mounted at /ram
Testing basic file operations
File operations test passed
Testing directory operations
Directory operations test passed
...
SUITE PASS - 100.00% [vfs_ramfs]: pass: 8, fail: 0, skip: 0
```

## Configuration

Key configuration options in `prj.conf`:
- `CONFIG_FILE_SYSTEM=y` - Enable Zephyr filesystem support
- `CONFIG_VFS_CORE=y` - Enable VFS core layer
- `CONFIG_RAMFS=y` - Enable RAMFS driver
- `CONFIG_HEAP_MEM_POOL_SIZE=16384` - Heap size for dynamic allocation

## Features

### VFS Core Layer
- Inode-based abstraction inspired by embox
- Reference counting for resource management
- Modular driver architecture
- Thread-safe operations with mutexes

### RAMFS Driver
- Fully in-memory filesystem
- Dynamic memory allocation
- Directory hierarchy support
- Standard POSIX-like operations

### Zephyr Integration
- Compatible with Zephyr's `fs_*` API
- Works with existing Zephyr filesystem infrastructure
- Can be mounted alongside FAT, LittleFS, etc.

## Limitations

- RAMFS data is lost on unmount or reboot
- Maximum filesystem size configurable via `CONFIG_RAMFS_MAX_SIZE`
- No persistence support (by design)

## Future Enhancements

- Add support for file permissions and ownership
- Implement symbolic links
- Add filesystem quotas
- Optimize memory usage for embedded systems
- Add more filesystem drivers (e.g., simple persistent FS)
