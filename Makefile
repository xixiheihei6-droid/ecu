.PHONY: build flash serial dev clean help keys setup lint fmt tidy tidy-ci check-clang-tidy install-deps

SERIAL_PORT ?= /dev/ttyUSB0
BAUD_RATE ?= 115200
UNAME := $(shell uname)
JOBS ?= $(shell nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)
APP_ADDR ?= 0x08004000
BOOTSTUB_ADDR ?= 0x08000000
APP_BIN ?= board/obj/ecu.bin.signed
BOOTSTUB_BIN ?= board/obj/bootstub.ecu.bin
C_FILES := $(shell find board \( -name '*.c' -o -name '*.h' \) ! -path 'board/inc/*' ! -path 'board/obj/*')
TIDY_FILES := $(shell find board -name '*.c' ! -path 'board/inc/*' ! -path 'board/obj/*')

build:
	scons -j$(JOBS) APP_START_ADDRESS=$(APP_ADDR)

help:
	@echo "Usage: make [target]"
	@echo ""
	@echo "Targets:"
	@echo "  build   Build firmware"
	@echo "  flash   Build and flash to device"
	@echo "  serial  Open serial monitor"
	@echo "  dev     Flash and open serial monitor"
	@echo "  clean   Clean build artifacts"
	@echo "  keys    Generate signing keypair (first time only)"
	@echo "  setup   Create venv and install Python dependencies"
	@echo "  lint    Run ruff and clang-format checks"
	@echo "  fmt     Auto-format C code with clang-format"
	@echo "  tidy    Run clang-tidy static analysis"
	@echo "  tidy-ci Run clang-tidy and fail on unexpected errors"
	@echo "  install-deps  Install system dependencies (clang-format, clang-tidy, ARM toolchain, openocd)"
	@echo ""
	@echo "Options:"
	@echo "  SERIAL_PORT=/dev/ttyUSB0  Serial port (default: /dev/ttyUSB0)"
	@echo "  BAUD_RATE=115200          Baud rate (default: 115200)"
	@echo "  APP_ADDR=0x08004000       Application flash base address"
	@echo "  BOOTSTUB_ADDR=0x08000000  Bootstub flash base address"
	@echo "  APP_BIN=board/obj/ecu.bin.signed         App image path"
	@echo "  BOOTSTUB_BIN=board/obj/bootstub.ecu.bin  Bootstub image path"

keys:
	python3 tools/gen_keypair.py

setup:
	python3 -m venv venv
	venv/bin/python -m pip install -r requirements.txt

lint:
	@command -v clang-format >/dev/null || (echo "Error: clang-format not found. Run: make install-deps" && exit 1)
	ruff check .
	clang-format --dry-run --Werror $(C_FILES)

fmt:
	clang-format -i $(C_FILES)

# Treat vendor headers as system headers to reduce clang-tidy noise from HAL code.
TIDY_FLAGS = -isystem board/inc/STM32F4xx_HAL_Driver/Inc -isystem board/inc \
	-I. -Iboard -DSTM32F4 -DSTM32F413xx

tidy: check-clang-tidy
	@echo "Note: clang-tidy warnings are advisory in this target"
	-clang-tidy --quiet $(TIDY_FILES) -- $(TIDY_FLAGS)

tidy-ci: check-clang-tidy
	@out=$$(mktemp); \
	clang-tidy --quiet $(TIDY_FILES) -- $(TIDY_FLAGS) > $$out 2>&1 || true; \
	cat $$out; \
	! grep -E "error:" $$out >/dev/null || \
		( echo "Unexpected clang-tidy errors found."; rm -f $$out; exit 1 ); \
	echo "clang-tidy CI check passed (no errors found)."; \
	rm -f $$out

check-clang-tidy:
	@command -v clang-tidy >/dev/null || (echo "clang-tidy not found; installing system dependencies..." && $(MAKE) install-deps)
	@command -v clang-tidy >/dev/null || (echo "Error: clang-tidy install failed." && exit 1)

install-deps:
ifeq ($(UNAME),Linux)
	sudo apt-get update && sudo apt-get install -y clang-format clang-tidy gcc-arm-none-eabi openocd
endif
ifeq ($(UNAME),Darwin)
	brew install clang-format llvm openocd && brew install --cask gcc-arm-embedded
endif

flash: build
	APP_ADDR=$(APP_ADDR) BOOTSTUB_ADDR=$(BOOTSTUB_ADDR) APP_BIN=$(APP_BIN) BOOTSTUB_BIN=$(BOOTSTUB_BIN) ./tools/recover.sh

serial:
	stty -F $(SERIAL_PORT) $(BAUD_RATE)
	cat $(SERIAL_PORT)

dev: flash serial

clean:
	scons -c
	rm -rf board/obj
	rm -f board/*.o
