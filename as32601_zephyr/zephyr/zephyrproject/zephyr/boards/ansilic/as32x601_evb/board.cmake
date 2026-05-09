# SPDX-License-Identifier: Apache-2.0

if(NOT "${OPENOCD}" MATCHES ".*openocd-as32x601$")
  set(OPENOCD OPENOCD-NOTFOUND)
endif()
find_program(OPENOCD openocd-as32x601
  PATHS $ENV{ZEPHYR_SDK_INSTALL_DIR}/sysroots/x86_64-pokysdk-linux/usr/bin
  NO_DEFAULT_PATH)

board_runner_args(openocd
  "--config" "${ZEPHYR_BASE}/boards/ansilic/as32x601_evb/support/openocd.cfg"
  "--cmd-reset-halt" "reset halt"
  "--cmd-pre-init" "adapter speed 1000"
)
include(${ZEPHYR_BASE}/boards/common/openocd.board.cmake)
