#!/usr/bin/env bash
set -e
DIR="$(cd "$(dirname "$0")" && pwd)"
BIN="${1:-$DIR/firmware/AS32_project.bin}"
ADDR="${2:-0x01000000}"
export LD_LIBRARY_PATH="$DIR/lib:${LD_LIBRARY_PATH:-}"
exec "$DIR/bin/openocd" -f "$DIR/cfg/as32x601_eflash.cfg" -c "program $BIN $ADDR verify reset exit"
