# ANL 轻量级 Loader / Linker 方案（RV32I 原生，类 ELF）

## 1. 目标

为 ansilic/AS32x601 设计极简二进制格式 **ANL**（ANsiLic），仅支持 **RV32I 原生代码**，
含静态重定位与动态符号解析。配套 Python linker 和 Zephyr C loader，不修改任何已有文件。

---

## 2. 整体流程

```
.c → riscv32-zephyr-elf-gcc -c → .o (ELF)
                                      │
                              tools/anl_link.py
                                      │
                               .anl 文件
                                      │
                         tools/upload/upload_hex.py
                                      │
                              板子 anl_loader.c
                         ┌────────────┴────────────┐
                      重定位修补              动态符号解析
                                      │
                              跳转 entry_off 执行
```

---

## 3. ANL 文件格式

### 3.1 文件头（FileHeader，32 字节，小端）

| 偏移 | 大小 | 字段        | 说明 |
|------|------|-------------|------|
| 0    | 4    | magic       | `0x4C4E417F`（`\x7FANL`，仿 ELF magic） |
| 4    | 1    | version     | 格式版本，当前 = 1 |
| 5    | 1    | arch        | 固定 = 1（RV32I） |
| 6    | 2    | flags       | ANL_F_PIC \| ANL_F_DYNAMIC \| ANL_F_STRIP |
| 8    | 4    | entry_off   | 入口点相对 SHT_CODE 节数据起始的字节偏移 |
| 12   | 2    | shnum       | 节头数量（含 SHT_NULL 占位节） |
| 14   | 2    | shentsize   | 节头大小，固定 = 24 |
| 16   | 4    | file_size   | 整个文件字节数 |
| 20   | 4    | crc32       | 全文件 CRC-32（计算时本字段置 0） |
| 24   | 4    | symtab_idx  | 静态符号表节的节头索引（无则 0） |
| 28   | 4    | dynsym_idx  | 动态符号节的节头索引（无则 0） |

**flags 位定义：**

| 位 | 名称          | 含义 |
|----|---------------|------|
| 0  | ANL_F_PIC     | 位置无关代码，需运行时重定位 |
| 1  | ANL_F_DYNAMIC | 含外部符号依赖 |
| 2  | ANL_F_STRIP   | 已去除调试符号 |

---

### 3.2 节头（SectionHeader，24 字节，小端）

紧跟 FileHeader，共 `shnum` 个。

| 偏移 | 大小 | 字段       | 说明 |
|------|------|------------|------|
| 0    | 2    | sh_type    | 节类型（见 3.2.1） |
| 2    | 2    | sh_flags   | 节属性（见 3.2.2） |
| 4    | 4    | sh_offset  | 节数据相对文件起始的字节偏移（BSS 节为 0） |
| 8    | 4    | sh_size    | 节数据字节数（BSS 节为需分配的字节数） |
| 12   | 4    | sh_addr    | 加载地址提示（0 = PIC） |
| 16   | 4    | sh_link    | 关联节索引（RELA→SYMTAB，DYNSYM→DYNSTR） |
| 20   | 4    | sh_entsize | 条目大小（SYMTAB/DYNSYM/RELA 节有效，其余 0） |

#### 3.2.1 sh_type 枚举

| 值 | 名称        | 含义 |
|----|-------------|------|
| 0  | SHT_NULL    | 占位（首节固定为此） |
| 1  | SHT_CODE    | RV32I 可执行指令（`.text`） |
| 2  | SHT_RODATA  | 只读数据（`.rodata`） |
| 3  | SHT_DATA    | 可读写初始化数据（`.data`） |
| 4  | SHT_BSS     | 未初始化数据，无文件体，运行时清零 |
| 5  | SHT_SYMTAB  | 静态符号表 |
| 6  | SHT_STRTAB  | 静态符号字符串表 |
| 7  | SHT_RELA    | 显式加数重定位表（针对某个节） |
| 8  | SHT_DYNSYM  | 动态符号表（外部未定义符号） |
| 9  | SHT_DYNSTR  | 动态符号字符串表 |

#### 3.2.2 sh_flags 位

| 位 | 名称      | 含义 |
|----|-----------|------|
| 0  | SHF_EXEC  | 可执行 |
| 1  | SHF_WRITE | 可写 |
| 2  | SHF_ALLOC | 需加载到内存 |

---

### 3.3 符号表条目（SymEntry，16 字节，小端）

用于 SHT_SYMTAB 和 SHT_DYNSYM，`sh_entsize = 16`。

| 偏移 | 大小 | 字段      | 说明 |
|------|------|-----------|------|
| 0    | 4    | st_name   | 符号名在对应 STRTAB/DYNSTR 节中的字节偏移 |
| 4    | 4    | st_value  | 符号值（相对所在节起始的偏移） |
| 8    | 4    | st_size   | 符号大小（字节） |
| 12   | 1    | st_type   | 0=NOTYPE, 1=FUNC, 2=OBJECT, 3=SECTION |
| 13   | 1    | st_bind   | 0=LOCAL, 1=GLOBAL, 2=WEAK |
| 14   | 1    | st_shndx  | 所在节索引（`0xFF` = 外部未定义） |
| 15   | 1    | reserved  | 置 0 |

---

### 3.4 重定位条目（RelaEntry，12 字节，小端）

用于 SHT_RELA 节，`sh_entsize = 12`，`sh_link` 指向对应符号表节索引。

| 偏移 | 大小 | 字段      | 说明 |
|------|------|-----------|------|
| 0    | 4    | r_offset  | 需要修补的位置（相对目标节起始的字节偏移） |
| 4    | 4    | r_info    | 高 24 位 = 符号表索引，低 8 位 = 重定位类型 |
| 8    | 4    | r_addend  | 显式加数（有符号） |

**重定位类型（r_info 低 8 位）：**

| 值 | 名称           | 公式              | 说明 |
|----|----------------|-------------------|------|
| 1  | R_ANL_32       | S + A             | 32 位绝对地址 |
| 2  | R_ANL_PC32     | S + A - P         | 32 位 PC 相对 |
| 3  | R_ANL_HI20     | (S + A) >> 12     | LUI 高 20 位 |
| 4  | R_ANL_LO12_I   | (S + A) & 0xFFF   | ADDI/LD 低 12 位 |
| 5  | R_ANL_CALL     | S + A - P         | JAL/JALR 26 位跳转偏移 |

---

### 3.5 动态符号解析流程（ANL_F_DYNAMIC 置位时）

1. loader 扫描 SHT_DYNSYM，找到 `st_shndx == 0xFF`（外部未定义）的符号
2. 在 Zephyr 侧 `_anl_exports[]` 表中按名查找
3. 找到后将符号地址代入对应 SHT_RELA 条目完成修补
4. 全部解析完成后跳转入口执行

---

### 3.6 典型节布局

```
[FileHeader       32B ]
[SHdr #0 NULL         ]  ← 占位，仿 ELF
[SHdr #1 CODE         ]  SHF_EXEC|ALLOC
[SHdr #2 RODATA       ]  SHF_ALLOC
[SHdr #3 DATA         ]  SHF_WRITE|ALLOC
[SHdr #4 BSS          ]  SHF_WRITE|ALLOC, sh_offset=0
[SHdr #5 SYMTAB       ]  sh_link=6, sh_entsize=16
[SHdr #6 STRTAB       ]
[SHdr #7 RELA         ]  sh_link=5（针对 CODE 节）
[SHdr #8 DYNSYM       ]  sh_link=9, sh_entsize=16
[SHdr #9 DYNSTR       ]
[各节数据 ...]
```

---

## 4. 新增文件清单

| 文件 | 说明 |
|------|------|
| `tools/anl_link.py` | Python linker：ELF `.o` → `.anl`（含符号表、重定位、动态符号） |
| `zephyr/samples/ansilic/as32x601_shell_process/src/anl_loader.h` | C 头：格式常量 + API 声明 |
| `zephyr/samples/ansilic/as32x601_shell_process/src/anl_loader.c` | C 实现：validate / 重定位 / 动态解析 / 执行 |

**不修改任何已有文件。**

---

## 5. Python Linker（tools/anl_link.py）

### 接口

```bash
python3 tools/anl_link.py input.o output.anl [--entry SYMBOL]
```

### 逻辑

1. 用 `pyelftools` 解析输入 ELF `.o`
2. 提取 `.text`→SHT_CODE，`.rodata`→SHT_RODATA，`.data`→SHT_DATA，`.bss`→SHT_BSS
3. 提取 ELF 符号表 → ANL SHT_SYMTAB + SHT_STRTAB
4. 转换 ELF 重定位条目 → ANL RelaEntry → SHT_RELA
5. `st_shndx==SHN_UNDEF` 的符号 → SHT_DYNSYM + SHT_DYNSTR，flags |= ANL_F_DYNAMIC
6. 计算 CRC-32，写出

**依赖：** `pyelftools`（`pip install pyelftools`）

---

## 6. C Loader API

### 头文件（anl_loader.h）

```c
/* 校验 magic、版本、CRC-32、节偏移越界 */
int anl_validate(const uint8_t *buf, size_t len);

/* 查找指定类型的第一个节，返回数据指针和大小 */
const uint8_t *anl_section(const uint8_t *buf, uint16_t sh_type,
                            uint32_t *out_size);

/* 加载 RV32I ANL：分配内存 → 复制节 → 重定位 → 动态解析 → 跳转入口 */
int anl_load(const char *name, const uint8_t *buf, size_t len);
```

### anl_load 内部流程

```
anl_validate()
  ↓
k_heap_alloc 为所有 SHF_ALLOC 节分配连续内存
  ↓
复制 CODE / RODATA / DATA；BSS memset(0)
  ↓
处理 SHT_RELA：
  sym_val = 符号在已分配内存中的地址
  patch   = CODE基址 + r_offset
  按 r_type 写入修补值
  ↓
若 ANL_F_DYNAMIC：
  对每个 st_shndx==0xFF 的符号：
    在 _anl_exports[] 中按名查找
    找不到 → k_heap_free + return -ENOENT
    找到   → 触发对应 RELA 条目修补
  ↓
entry = CODE基址 + fhdr->entry_off
((void(*)(void))entry)()
```

### Zephyr 导出符号表

```c
struct anl_export { const char *name; uintptr_t addr; };
extern const struct anl_export _anl_exports[];
/* 初期手动注册：printk、k_msleep 等 */
```

---

## 7. 典型使用流程

```bash
riscv32-zephyr-elf-gcc -c -O2 -march=rv32i -mabi=ilp32 hello.c -o hello.o
python3 tools/anl_link.py hello.o /tmp/hello.anl --entry main
sudo python3 tools/upload/upload_hex.py /dev/ttyUSB0 115200 hello /tmp/hello.anl
```

---

*待审批后依次落盘：anl_loader.h → anl_loader.c → anl_link.py*
