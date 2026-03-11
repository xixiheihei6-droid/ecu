#!/usr/bin/env sh
set -e

INTERFACE="interface/stlink.cfg"
TARGET="target/stm32f4x.cfg"
APP_BIN="${APP_BIN:-board/obj/ecu.bin.signed}"
APP_ADDR="${APP_ADDR:-0x08004000}"
BOOTSTUB_BIN="${BOOTSTUB_BIN:-board/obj/bootstub.ecu.bin}"
BOOTSTUB_ADDR="${BOOTSTUB_ADDR:-0x08000000}"

openocd \
  -f "$INTERFACE" \
  -c "adapter speed 1800" \
  -c "set CPUTAPID 0" \
  -f "$TARGET" \
  -c "init" \
  -c "reset halt" \
  -c "flash write_image erase $APP_BIN $APP_ADDR" \
  -c "flash write_image erase $BOOTSTUB_BIN $BOOTSTUB_ADDR" \
  -c "reset run" \
  -c "shutdown"
