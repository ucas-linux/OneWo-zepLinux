# SPDX-License-Identifier: Apache-2.0

board_runner_args(openocd
  "--config" "${CMAKE_CURRENT_LIST_DIR}/support/openocd.cfg"
  "--cmd-reset-halt" "reset halt"
  "--cmd-pre-init" "adapter speed 1000"
)
include(${ZEPHYR_BASE}/boards/common/openocd.board.cmake)
