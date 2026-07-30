#include <zephyr/kernel.h>
#include <string.h>
#include "anl_loader.h"

/* CRC-32 (IEEE 802.3) */
static uint32_t crc32(const uint8_t *buf, size_t len)
{
    uint32_t crc = 0xFFFFFFFFU;
    for (size_t i = 0; i < len; i++) {
        crc ^= buf[i];
        for (int j = 0; j < 8; j++)
            crc = (crc >> 1) ^ (0xEDB88320U & -(crc & 1));
    }
    return ~crc;
}

int anl_validate(const uint8_t *buf, size_t len)
{
    if (len < sizeof(AnlFileHeader)) return -1;
    const AnlFileHeader *fh = (const AnlFileHeader *)buf;
    if (fh->magic != ANL_MAGIC) return -2;
    if (fh->version != ANL_VERSION) return -3;
    if (fh->file_size > len) return -4;

    /* CRC check: compute with crc32 field zeroed */
    uint8_t tmp[sizeof(uint32_t)];
    memcpy(tmp, &fh->crc32, 4);
    uint32_t stored = fh->crc32;
    /* We can't modify buf (const), so compute in two parts */
    uint32_t crc = 0xFFFFFFFFU;
    size_t crc_off = offsetof(AnlFileHeader, crc32);
    for (size_t i = 0; i < fh->file_size; i++) {
        uint8_t b = (i >= crc_off && i < crc_off + 4) ? 0 : buf[i];
        crc ^= b;
        for (int j = 0; j < 8; j++)
            crc = (crc >> 1) ^ (0xEDB88320U & -(crc & 1));
    }
    crc = ~crc;
    if (crc != stored) return -5;
    return 0;
}

const uint8_t *anl_section(const uint8_t *buf, uint16_t sh_type, uint32_t *out_size)
{
    const AnlFileHeader *fh = (const AnlFileHeader *)buf;
    const AnlSectionHeader *sh = (const AnlSectionHeader *)(buf + sizeof(AnlFileHeader));
    for (int i = 0; i < fh->shnum; i++) {
        if (sh[i].sh_type == sh_type) {
            if (out_size) *out_size = sh[i].sh_size;
            return buf + sh[i].sh_offset;
        }
    }
    return NULL;
}

static const AnlSectionHeader *get_shdr(const uint8_t *buf, int idx)
{
    const AnlFileHeader *fh = (const AnlFileHeader *)buf;
    if (idx < 0 || idx >= fh->shnum) return NULL;
    return (const AnlSectionHeader *)(buf + sizeof(AnlFileHeader)) + idx;
}

int anl_load(const char *name, const uint8_t *buf, size_t len)
{
    int ret = anl_validate(buf, len);
    if (ret) {
        printk("anl_load[%s]: validate failed %d\n", name, ret);
        return ret;
    }

    const AnlFileHeader *fh = (const AnlFileHeader *)buf;
    const AnlSectionHeader *shdrs = (const AnlSectionHeader *)(buf + sizeof(AnlFileHeader));

    /* Calculate total alloc size for SHF_ALLOC sections */
    uint32_t total = 0;
    for (int i = 0; i < fh->shnum; i++) {
        if (shdrs[i].sh_flags & SHF_ALLOC)
            total += shdrs[i].sh_size;
    }

    /* Allocate aligned memory (RISC-V requires 4-byte alignment, use 8 for safety) */
    uint8_t *mem = k_aligned_alloc(8, total);
    if (!mem) {
        printk("anl_load[%s]: OOM (%u bytes)\n", name, total);
        return -ENOMEM;
    }
    memset(mem, 0, total);

    printk("anl_load[%s]: allocated %u bytes at 0x%08x (aligned)\n",
           name, total, (unsigned)mem);

    /* Map section index -> runtime base address */
    uintptr_t sec_base[16] = {0};
    uint32_t off = 0;
    for (int i = 0; i < fh->shnum && i < 16; i++) {
        if (shdrs[i].sh_flags & SHF_ALLOC) {
            sec_base[i] = (uintptr_t)(mem + off);
            if (shdrs[i].sh_type != SHT_BSS)
                memcpy(mem + off, buf + shdrs[i].sh_offset, shdrs[i].sh_size);
            off += shdrs[i].sh_size;
        }
    }

    /* Find CODE section base */
    uintptr_t code_base = 0;
    for (int i = 0; i < fh->shnum && i < 16; i++) {
        if (shdrs[i].sh_type == SHT_CODE) { code_base = sec_base[i]; break; }
    }

    /* Process RELA sections */
    for (int i = 0; i < fh->shnum; i++) {
        if (shdrs[i].sh_type != SHT_RELA) continue;
        const AnlRelaEntry *relas = (const AnlRelaEntry *)(buf + shdrs[i].sh_offset);
        uint32_t nrela = shdrs[i].sh_size / sizeof(AnlRelaEntry);
        int symtab_idx = shdrs[i].sh_link;
        const AnlSectionHeader *symsh = get_shdr(buf, symtab_idx);
        if (!symsh) continue;
        const AnlSymEntry *syms = (const AnlSymEntry *)(buf + symsh->sh_offset);
        const AnlSectionHeader *strsh = get_shdr(buf, symsh->sh_link);
        const char *strtab = strsh ? (const char *)(buf + strsh->sh_offset) : NULL;

        for (uint32_t r = 0; r < nrela; r++) {
            uint32_t sym_idx = relas[r].r_info >> 8;
            uint8_t  r_type  = relas[r].r_info & 0xFF;
            const AnlSymEntry *sym = &syms[sym_idx];

            uintptr_t S = 0;
            if (sym->st_shndx == ANL_SHNDX_UNDEF) {
                /* Dynamic symbol lookup */
                const char *sname = strtab ? strtab + sym->st_name : "";
                for (int e = 0; e < _anl_exports_count; e++) {
                    if (strcmp(_anl_exports[e].name, sname) == 0) {
                        S = _anl_exports[e].addr;
                        break;
                    }
                }
                if (!S) {
                    printk("anl_load[%s]: unresolved symbol '%s'\n", name, sname);
                    k_free(mem);
                    return -ENOENT;
                }
            } else if (sym->st_shndx < 16) {
                S = sec_base[sym->st_shndx] + sym->st_value;
            }

            /* Patch location is relative to CODE section */
            uint32_t *patch = (uint32_t *)(code_base + relas[r].r_offset);
            int32_t   A     = relas[r].r_addend;
            uintptr_t P     = (uintptr_t)patch;

            printk("  rela[%u] type=%u sym=%u S=0x%08x P=0x%08x A=%d\n",
                   r, r_type, sym_idx, (unsigned)S, (unsigned)P, A);

            uint32_t orig = *patch;
            switch (r_type) {
            case R_ANL_32:
            case R_ANL_ABS32:
                *patch = (uint32_t)(S + A);
                break;
            case R_ANL_PC32:
                *patch = (uint32_t)(S + A - P);
                break;
            case R_ANL_THM_CALL: {
                /* ARM Thumb BL: encode 26-bit offset into two 16-bit halfwords */
                int32_t offset = (int32_t)(S + A - P - 4);
                uint16_t *hw = (uint16_t *)patch;
                uint32_t s  = (offset >> 24) & 1;
                uint32_t i1 = (offset >> 23) & 1;
                uint32_t i2 = (offset >> 22) & 1;
                uint32_t j1 = (~(i1 ^ s)) & 1;
                uint32_t j2 = (~(i2 ^ s)) & 1;
                uint32_t imm10 = (offset >> 12) & 0x3FF;
                uint32_t imm11 = (offset >> 1)  & 0x7FF;
                hw[0] = (uint16_t)(0xF000 | (s << 10) | imm10);
                hw[1] = (uint16_t)(0xF800 | (j1 << 13) | (j2 << 11) | imm11);
                break;
            }
            case R_ANL_HI20: {
                /* RISC-V LUI: upper 20 bits in [31:12] */
                uint32_t hi = (uint32_t)((S + A + 0x800) >> 12) & 0xFFFFF;
                *patch = (*patch & 0xFFF) | (hi << 12);
                break;
            }
            case R_ANL_LO12_I: {
                /* RISC-V I-type: 12-bit signed immediate in [31:20] */
                uint32_t lo = (uint32_t)(S + A) & 0xFFF;
                *patch = (*patch & 0x000FFFFF) | (lo << 20);
                break;
            }
            case R_ANL_CALL: {
                /* RISC-V CALL: AUIPC + JALR pair
                 * Offset 0: AUIPC rd, %pcrel_hi(symbol)
                 * Offset 4: JALR rd, %pcrel_lo(symbol)(rd)
                 */
                int32_t offset = (int32_t)(S + A - P);

                /* Split into high 20 bits and low 12 bits */
                int32_t hi = (offset + 0x800) >> 12;  /* Add 0x800 for rounding */
                int32_t lo = offset & 0xFFF;

                /* Patch AUIPC instruction at patch[0] */
                uint32_t *auipc = patch;
                uint32_t orig_auipc = *auipc;
                *auipc = (*auipc & 0xFFF) | ((hi & 0xFFFFF) << 12);

                /* Patch JALR instruction at patch[1] */
                uint32_t *jalr = patch + 1;
                uint32_t orig_jalr = *jalr;
                *jalr = (*jalr & 0x000FFFFF) | ((lo & 0xFFF) << 20);

                printk("    CALL: offset=0x%x hi=0x%x lo=0x%x\n", offset, hi, lo);
                printk("    auipc: 0x%08x -> 0x%08x\n", orig_auipc, *auipc);
                printk("    jalr:  0x%08x -> 0x%08x\n", orig_jalr, *jalr);

                break;
            }
            default:
                printk("anl_load[%s]: unknown reloc type %d\n", name, r_type);
                break;
            }
            printk("    patched: 0x%08x -> 0x%08x\n", orig, *patch);
        }
    }

    /* Jump to entry */
    uintptr_t entry = code_base + fh->entry_off;
    printk("anl_load[%s]: jumping to entry 0x%08x\n", name, (unsigned)entry);

    /* Debug: print first few instructions */
    uint32_t *code = (uint32_t *)code_base;
    printk("  code[0]=0x%08x code[1]=0x%08x code[2]=0x%08x\n",
           code[0], code[1], code[2]);
    printk("  code[3]=0x%08x code[4]=0x%08x code[5]=0x%08x\n",
           code[3], code[4], code[5]);

    /* Ensure all writes are visible before execution:
     * fence   - complete all prior stores (data ordering)
     * fence.i - flush I-cache so instruction fetch sees new code */
    __asm__ volatile ("fence" ::: "memory");
    __asm__ volatile ("fence.i" ::: "memory");

    /* Set Thumb bit for ARM, no modification for RISC-V */
    if (fh->arch == ANL_ARCH_THUMB2) {
        entry |= 1;  /* ARM Thumb mode requires LSB set */
    }

    void (*fn)(void) = (void (*)(void))entry;
    fn();

    /* Note: mem is intentionally not freed here because spawned child
     * threads may still be executing code from this module. */
    return 0;
}
