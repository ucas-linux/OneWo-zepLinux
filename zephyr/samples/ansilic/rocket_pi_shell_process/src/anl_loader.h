#ifndef ANL_LOADER_H
#define ANL_LOADER_H

#include <stdint.h>
#include <stddef.h>

/* Magic: \x7FANL */
#define ANL_MAGIC        0x4C4E417FU
#define ANL_VERSION      1
#define ANL_ARCH_RV32I   1
#define ANL_ARCH_THUMB2  2

/* File flags */
#define ANL_F_PIC        (1 << 0)
#define ANL_F_DYNAMIC    (1 << 1)
#define ANL_F_STRIP      (1 << 2)

/* Section types */
#define SHT_NULL    0
#define SHT_CODE    1
#define SHT_RODATA  2
#define SHT_DATA    3
#define SHT_BSS     4
#define SHT_SYMTAB  5
#define SHT_STRTAB  6
#define SHT_RELA    7
#define SHT_DYNSYM  8
#define SHT_DYNSTR  9

/* Section flags */
#define SHF_EXEC    (1 << 0)
#define SHF_WRITE   (1 << 1)
#define SHF_ALLOC   (1 << 2)

/* Relocation types (RISC-V RV32I and ARM Thumb2) */
#define R_ANL_32        1
#define R_ANL_PC32      2
#define R_ANL_THM_CALL  3  /* Thumb BL/BLX */
#define R_ANL_ABS32     4
#define R_ANL_HI20      5  /* RISC-V LUI high 20 bits */
#define R_ANL_LO12_I    6  /* RISC-V ADDI/LD low 12 bits */
#define R_ANL_CALL      7  /* RISC-V JAL/JALR */

#define ANL_SHNDX_UNDEF 0xFF

#pragma pack(push, 1)

typedef struct {
    uint32_t magic;
    uint8_t  version;
    uint8_t  arch;
    uint16_t flags;
    uint32_t entry_off;
    uint16_t shnum;
    uint16_t shentsize;
    uint32_t file_size;
    uint32_t crc32;
    uint32_t symtab_idx;
    uint32_t dynsym_idx;
} AnlFileHeader;  /* 32 bytes */

typedef struct {
    uint16_t sh_type;
    uint16_t sh_flags;
    uint32_t sh_offset;
    uint32_t sh_size;
    uint32_t sh_addr;
    uint32_t sh_link;
    uint32_t sh_entsize;
} AnlSectionHeader;  /* 24 bytes */

typedef struct {
    uint32_t st_name;
    uint32_t st_value;
    uint32_t st_size;
    uint8_t  st_type;
    uint8_t  st_bind;
    uint8_t  st_shndx;
    uint8_t  reserved;
} AnlSymEntry;  /* 16 bytes */

typedef struct {
    uint32_t r_offset;
    uint32_t r_info;
    int32_t  r_addend;
} AnlRelaEntry;  /* 12 bytes */

#pragma pack(pop)

struct anl_export {
    const char *name;
    uintptr_t   addr;
};

extern const struct anl_export _anl_exports[];
extern const int _anl_exports_count;

int anl_validate(const uint8_t *buf, size_t len);
const uint8_t *anl_section(const uint8_t *buf, uint16_t sh_type, uint32_t *out_size);
int anl_load(const char *name, const uint8_t *buf, size_t len);

#endif /* ANL_LOADER_H */
