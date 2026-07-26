SHELL := /bin/bash

SAMPLE_DIR := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
WEST_TOPDIR := $(abspath $(SAMPLE_DIR)/../../../..)

BOARD ?= qemu_cortex_m3
BUILD_DIR ?= $(WEST_TOPDIR)/build-anl-qemu
MODULE ?= hello
ENTRY ?= main
MODULE_SRC ?= $(SAMPLE_DIR)/examples/$(MODULE).c
MODULE_BUILD_DIR ?= $(SAMPLE_DIR)/build/modules
MODULE_OBJ := $(MODULE_BUILD_DIR)/$(MODULE).o
MODULE_ANL := $(MODULE_BUILD_DIR)/$(MODULE).anl

ZEPHYR_SDK_INSTALL_DIR ?= /home/zzy/zephyr-sdk-0.17.4
ARM_GCC ?= $(ZEPHYR_SDK_INSTALL_DIR)/arm-zephyr-eabi/bin/arm-zephyr-eabi-gcc
PYTHON ?= python3
WEST ?= west

MODULE_CFLAGS ?= -mthumb -mcpu=cortex-m3 -O1 -ffreestanding -mlong-calls
QEMU_START_DELAY ?= 2
MODULE_RUN_DELAY ?= 3

.PHONY: all loader module run clean help check-tools

all: loader module

loader: check-tools
	$(WEST) build -p always -b $(BOARD) -d $(BUILD_DIR) $(SAMPLE_DIR)

module: $(MODULE_ANL)

$(MODULE_BUILD_DIR):
	mkdir -p $@

$(MODULE_OBJ): $(MODULE_SRC) | $(MODULE_BUILD_DIR) check-tools
	$(ARM_GCC) $(MODULE_CFLAGS) -c $< -o $@

$(MODULE_ANL): $(MODULE_OBJ) $(WEST_TOPDIR)/tools/anl_link.py
	$(PYTHON) $(WEST_TOPDIR)/tools/anl_link.py $< $@ --entry $(ENTRY)
	@printf 'Generated %s (%s bytes)\n' '$@' "$$(wc -c < '$@')"

run: all
	@hex="$$($(PYTHON) -c "print(open('$(MODULE_ANL)', 'rb').read().hex().upper())")"; \
	name='$(MODULE)'; \
	if (( $${#hex} + $${#name} + 7 >= 8256 )); then \
		echo "ERROR: load command exceeds the loader input buffer" >&2; \
		exit 1; \
	fi; \
	echo "Loading $(MODULE_ANL) into QEMU as '$(MODULE)'..."; \
	( sleep $(QEMU_START_DELAY); \
	  printf 'load %s %s\r' '$(MODULE)' "$$hex"; \
	  sleep $(MODULE_RUN_DELAY); \
	  printf '\001x'; \
	) | $(WEST) build -d $(BUILD_DIR) -t run

check-tools:
	@command -v $(WEST) >/dev/null || { echo "ERROR: west not found: $(WEST)" >&2; exit 1; }
	@test -x "$(ARM_GCC)" || { echo "ERROR: ARM GCC not executable: $(ARM_GCC)" >&2; exit 1; }
	@$(PYTHON) -c 'import elftools' >/dev/null 2>&1 || { \
		echo "ERROR: pyelftools is missing; run: $(PYTHON) -m pip install pyelftools" >&2; \
		exit 1; \
	}

clean:
	rm -rf $(MODULE_BUILD_DIR) $(BUILD_DIR)

help:
	@printf '%s\n' \
	  'ANL Loader QEMU targets:' \
	  '  make          Build the QEMU loader and hello.anl' \
	  '  make module   Build only the ANL module' \
	  '  make loader   Build only the Zephyr QEMU loader' \
	  '  make run      Build everything, load the module, and run QEMU' \
	  '  make clean    Remove loader and module build outputs' \
	  '' \
	  'Common overrides:' \
	  '  make run MODULE=hello' \
	  '  make run ARM_GCC=/path/to/arm-zephyr-eabi-gcc' \
	  '  make run ZEPHYR_SDK_INSTALL_DIR=/path/to/zephyr-sdk'
