# ELF解析器实现

## 数据结构

```c
typedef struct {
    const uint8_t *elf_data;    // ELF文件数据
    size_t elf_size;            // 文件大小

    Elf32_Ehdr *ehdr;           // ELF header
    Elf32_Phdr *phdr;           // Program headers
    Elf32_Shdr *shdr;           // Section headers

    uint32_t entry;             // 入口点地址

    // 段信息
    struct {
        uint32_t vaddr;         // 虚拟地址
        uint32_t offset;        // 文件偏移
        uint32_t filesz;        // 文件大小
        uint32_t memsz;         // 内存大小
        uint32_t flags;         // 权限标志
    } segments[CONFIG_ELF_MAX_SEGMENTS];
    int num_segments;

    // 重定位信息
    Elf32_Rel *rel;
    int num_rel;

    // 符号表
    Elf32_Sym *symtab;
    int num_syms;
    const char *strtab;

} elf_handle_t;
```

## 解析流程

```c
int elf_parse(const char *path, elf_handle_t *handle)
{
    int ret;

    // 1. 读取ELF文件
    ret = elf_read_file(path, handle);
    if (ret < 0) {
        return ret;
    }

    // 2. 验证ELF header
    ret = elf_validate_header(handle);
    if (ret < 0) {
        return ret;
    }

    // 3. 解析Program headers
    ret = elf_parse_program_headers(handle);
    if (ret < 0) {
        return ret;
    }

    // 4. 解析Section headers
    ret = elf_parse_section_headers(handle);
    if (ret < 0) {
        return ret;
    }

    // 5. 查找符号表和字符串表
    ret = elf_find_symtab(handle);
    if (ret < 0) {
        // 符号表可选，不是致命错误
        ELF_DEBUG("No symbol table found\n");
    }

    // 6. 查找重定位表
    ret = elf_find_relocations(handle);
    if (ret < 0) {
        ELF_DEBUG("No relocations found\n");
    }

    return 0;
}
```

## 1. 读取ELF文件

```c
static int elf_read_file(const char *path, elf_handle_t *handle)
{
    struct fs_file_t file;
    struct fs_dirent entry;
    int ret;

    // 获取文件大小
    ret = fs_stat(path, &entry);
    if (ret < 0) {
        return -ENOENT;
    }

    handle->elf_size = entry.size;

    // 分配内存
    handle->elf_data = k_malloc(handle->elf_size);
    if (!handle->elf_data) {
        return -ENOMEM;
    }

    // 打开文件
    fs_file_t_init(&file);
    ret = fs_open(&file, path, FS_O_READ);
    if (ret < 0) {
        k_free((void *)handle->elf_data);
        return ret;
    }

    // 读取整个文件
    ssize_t bytes_read = fs_read(&file, (void *)handle->elf_data,
                                  handle->elf_size);
    fs_close(&file);

    if (bytes_read != handle->elf_size) {
        k_free((void *)handle->elf_data);
        return -EIO;
    }

    return 0;
}
```

## 2. 验证ELF Header

```c
static int elf_validate_header(elf_handle_t *handle)
{
    Elf32_Ehdr *ehdr = (Elf32_Ehdr *)handle->elf_data;

    // 检查文件大小
    if (handle->elf_size < sizeof(Elf32_Ehdr)) {
        ELF_ERROR("File too small\n");
        return -ENOEXEC;
    }

    // 检查魔数
    if (ehdr->e_ident[EI_MAG0] != ELFMAG0 ||
        ehdr->e_ident[EI_MAG1] != ELFMAG1 ||
        ehdr->e_ident[EI_MAG2] != ELFMAG2 ||
        ehdr->e_ident[EI_MAG3] != ELFMAG3) {
        ELF_ERROR("Invalid ELF magic\n");
        return -ENOEXEC;
    }

    // 检查类别（32位）
    if (ehdr->e_ident[EI_CLASS] != ELFCLASS32) {
        ELF_ERROR("Not 32-bit ELF\n");
        return -ENOEXEC;
    }

    // 检查字节序（小端）
    if (ehdr->e_ident[EI_DATA] != ELFDATA2LSB) {
        ELF_ERROR("Not little-endian\n");
        return -ENOEXEC;
    }

    // 检查版本
    if (ehdr->e_ident[EI_VERSION] != EV_CURRENT) {
        ELF_ERROR("Invalid ELF version\n");
        return -ENOEXEC;
    }

    // 检查架构（ARM）
    if (ehdr->e_machine != EM_ARM) {
        ELF_ERROR("Not ARM architecture (machine=%d)\n", ehdr->e_machine);
        return -ENOEXEC;
    }

    // 检查文件类型（可执行文件）
    if (ehdr->e_type != ET_EXEC) {
        ELF_ERROR("Not executable (type=%d)\n", ehdr->e_type);
        return -ENOEXEC;
    }

    // 检查入口点
    if (ehdr->e_entry == 0) {
        ELF_ERROR("No entry point\n");
        return -ENOEXEC;
    }

    // 检查是否静态链接（无PT_INTERP）
    // 这个在解析program headers时检查

    handle->ehdr = ehdr;
    handle->entry = ehdr->e_entry;

    ELF_DEBUG("ELF validated: entry=0x%08x\n", handle->entry);

    return 0;
}
```

## 3. 解析Program Headers

```c
static int elf_parse_program_headers(elf_handle_t *handle)
{
    Elf32_Ehdr *ehdr = handle->ehdr;
    Elf32_Phdr *phdr;
    int i;

    // 检查program header偏移和大小
    if (ehdr->e_phoff == 0 || ehdr->e_phnum == 0) {
        ELF_ERROR("No program headers\n");
        return -ENOEXEC;
    }

    if (ehdr->e_phoff + ehdr->e_phnum * sizeof(Elf32_Phdr) > handle->elf_size) {
        ELF_ERROR("Program headers out of bounds\n");
        return -ENOEXEC;
    }

    phdr = (Elf32_Phdr *)(handle->elf_data + ehdr->e_phoff);
    handle->phdr = phdr;
    handle->num_segments = 0;

    // 遍历program headers
    for (i = 0; i < ehdr->e_phnum; i++) {
        ELF_DEBUG("Program header %d: type=%d vaddr=0x%08x filesz=%u memsz=%u flags=0x%x\n",
                  i, phdr[i].p_type, phdr[i].p_vaddr,
                  phdr[i].p_filesz, phdr[i].p_memsz, phdr[i].p_flags);

        // 检查PT_INTERP（动态链接器）
        if (phdr[i].p_type == PT_INTERP) {
            ELF_ERROR("Dynamic linking not supported (PT_INTERP found)\n");
            return -ENOEXEC;
        }

        // 检查PT_DYNAMIC（动态链接信息）
        if (phdr[i].p_type == PT_DYNAMIC) {
            ELF_ERROR("Dynamic linking not supported (PT_DYNAMIC found)\n");
            return -ENOEXEC;
        }

        // 收集PT_LOAD段
        if (phdr[i].p_type == PT_LOAD) {
            if (handle->num_segments >= CONFIG_ELF_MAX_SEGMENTS) {
                ELF_ERROR("Too many loadable segments\n");
                return -ENOEXEC;
            }

            // 验证段
            if (phdr[i].p_filesz > phdr[i].p_memsz) {
                ELF_ERROR("Invalid segment: filesz > memsz\n");
                return -ENOEXEC;
            }

            if (phdr[i].p_offset + phdr[i].p_filesz > handle->elf_size) {
                ELF_ERROR("Segment data out of bounds\n");
                return -ENOEXEC;
            }

            // 保存段信息
            handle->segments[handle->num_segments].vaddr = phdr[i].p_vaddr;
            handle->segments[handle->num_segments].offset = phdr[i].p_offset;
            handle->segments[handle->num_segments].filesz = phdr[i].p_filesz;
            handle->segments[handle->num_segments].memsz = phdr[i].p_memsz;
            handle->segments[handle->num_segments].flags = phdr[i].p_flags;
            handle->num_segments++;
        }
    }

    if (handle->num_segments == 0) {
        ELF_ERROR("No loadable segments\n");
        return -ENOEXEC;
    }

    ELF_DEBUG("Found %d loadable segments\n", handle->num_segments);

    return 0;
}
```

## 4. 解析Section Headers

```c
static int elf_parse_section_headers(elf_handle_t *handle)
{
    Elf32_Ehdr *ehdr = handle->ehdr;
    Elf32_Shdr *shdr;

    // Section headers是可选的（对于stripped binary）
    if (ehdr->e_shoff == 0 || ehdr->e_shnum == 0) {
        ELF_DEBUG("No section headers (stripped binary)\n");
        handle->shdr = NULL;
        return 0;
    }

    if (ehdr->e_shoff + ehdr->e_shnum * sizeof(Elf32_Shdr) > handle->elf_size) {
        ELF_ERROR("Section headers out of bounds\n");
        return -ENOEXEC;
    }

    shdr = (Elf32_Shdr *)(handle->elf_data + ehdr->e_shoff);
    handle->shdr = shdr;

    ELF_DEBUG("Found %d section headers\n", ehdr->e_shnum);

    return 0;
}
```

## 5. 查找符号表

```c
static int elf_find_symtab(elf_handle_t *handle)
{
    Elf32_Ehdr *ehdr = handle->ehdr;
    Elf32_Shdr *shdr = handle->shdr;
    int i;

    if (!shdr) {
        return -ENOENT;
    }

    // 查找符号表和字符串表
    for (i = 0; i < ehdr->e_shnum; i++) {
        if (shdr[i].sh_type == SHT_SYMTAB) {
            handle->symtab = (Elf32_Sym *)(handle->elf_data + shdr[i].sh_offset);
            handle->num_syms = shdr[i].sh_size / sizeof(Elf32_Sym);

            // 获取关联的字符串表
            int strtab_idx = shdr[i].sh_link;
            if (strtab_idx < ehdr->e_shnum) {
                handle->strtab = (const char *)(handle->elf_data +
                                                 shdr[strtab_idx].sh_offset);
            }

            ELF_DEBUG("Found symbol table: %d symbols\n", handle->num_syms);
            return 0;
        }
    }

    return -ENOENT;
}
```

## 6. 查找重定位表

```c
static int elf_find_relocations(elf_handle_t *handle)
{
    Elf32_Ehdr *ehdr = handle->ehdr;
    Elf32_Shdr *shdr = handle->shdr;
    int i;

    if (!shdr) {
        return -ENOENT;
    }

    // 查找重定位表
    for (i = 0; i < ehdr->e_shnum; i++) {
        if (shdr[i].sh_type == SHT_REL) {
            handle->rel = (Elf32_Rel *)(handle->elf_data + shdr[i].sh_offset);
            handle->num_rel = shdr[i].sh_size / sizeof(Elf32_Rel);

            ELF_DEBUG("Found relocation table: %d entries\n", handle->num_rel);
            return 0;
        }
    }

    return -ENOENT;
}
```

## 清理资源

```c
void elf_close(elf_handle_t *handle)
{
    if (handle->elf_data) {
        k_free((void *)handle->elf_data);
        handle->elf_data = NULL;
    }

    memset(handle, 0, sizeof(*handle));
}
```

## 调试辅助

```c
void elf_dump_info(elf_handle_t *handle)
{
    printk("=== ELF Information ===\n");
    printk("Entry point: 0x%08x\n", handle->entry);
    printk("Segments: %d\n", handle->num_segments);

    for (int i = 0; i < handle->num_segments; i++) {
        printk("  Segment %d:\n", i);
        printk("    vaddr:  0x%08x\n", handle->segments[i].vaddr);
        printk("    filesz: %u\n", handle->segments[i].filesz);
        printk("    memsz:  %u\n", handle->segments[i].memsz);
        printk("    flags:  0x%x ", handle->segments[i].flags);

        if (handle->segments[i].flags & PF_R) printk("R");
        if (handle->segments[i].flags & PF_W) printk("W");
        if (handle->segments[i].flags & PF_X) printk("X");
        printk("\n");
    }

    if (handle->symtab) {
        printk("Symbols: %d\n", handle->num_syms);
    }

    if (handle->rel) {
        printk("Relocations: %d\n", handle->num_rel);
    }
}
```
