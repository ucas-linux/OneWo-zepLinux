OpenOCD AS32x601 Linux package

Files:
- bin/openocd                : executable
- lib/                       : bundled local runtime libs
- cfg/as32x601_eflash.cfg    : validated target cfg
- firmware/AS32_project.bin  : tested sample firmware
- run_openocd.sh             : start OpenOCD with cfg
- flash_bin.sh               : program a BIN file

Usage:
1) Start OpenOCD:
   ./run_openocd.sh

2) Program bundled BIN:
   ./flash_bin.sh

3) Program your own BIN:
   ./flash_bin.sh /path/to/your.bin 0x01000000
