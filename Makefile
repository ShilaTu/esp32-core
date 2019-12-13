#
# see .Makefile.template for help
#

### variables ###

VARIABLE+=USERSHELL
HELP_USERSHELL=what shell to spawn when setting env
USERSHELL?=$(shell awk -F ':' '/^'$$(id -un)':/{print $$NF}' /etc/passwd)

VARIABLE+=DEV
HELP_DEV=which device to flash/monitor
DEV?=none

### build targets ###

.PHONY: build
TARGET += build
DEFAULT += build
ALL += build
HELP_build = builds project
build: | check-docker
	@make --no-print-directory -C docker pio EXEC="pio run"

.PHONY: clean-build
CLEAN += clean-build
HELP_clean-build = let pio clean generated files
clean-build: | check-docker
	@make --no-print-directory -C docker pio EXEC="pio run -t clean"

.PHONY: distclean-build
DISTCLEAN += distclean-build
HELP_distclean-build = removes all generated files by platformio
distclean-build: clean-build
	rm -rf .pio

### flash targets ###

.PHONY: flash
TARGET += flash
HELP_flash = flashes project to esp. Use DEV=path to provide  path to device or use make dev
flash: | check-flash
	@make --no-print-directory -C docker \
		pio \
		EXEC="sudo chgrp developer $(DEV); pio run --upload-port '$(DEV)' -t upload" DEV="$(DEV)"

.PHONY: check-flash
CHECK += check-flash
HELP_check-flash = checks env if flashing is possible
check-flash: | check-docker check-dev

### monitor targets ###

.PHONY: monitor
TARGET += monitor
HELP_monitor = connects to esp32 via serial. Use DEV=path to provide path to device or use make dev
monitor: | check-monitor
	@make --no-print-directory -C docker \
		pio \
		EXEC="sudo chgrp developer $(DEV); \
			python3 \
				/home/developer/.platformio/packages/framework-espidf/tools/idf_monitor.py \
				.pio/build/pico32/firmware.elf \
				--port $(DEV)" \
		DEV=$(DEV)

.PHONY: check-monitor
CHECK += check-monitor
HELP_check-monitor = checks env if monitor is possible
check-monitor: | check-docker check-dev

### dev targets ###

.PHONY: dev
TARGET += dev
HELP_dev = specifies which USB device to connect to
dev:
ifeq (,$(shell which dialog))
	$(error dialog is not installed)
endif
	export DEV=$$((for dev in $$(ls /dev/serial/by-id); do echo "$$(readlink -f /dev/serial/by-id/$$dev) $$dev "; done ) \
	| dialog \
		--stdout \
		--menu "choose which device to use" 0 0 0 "none" "disable device passthrough"\
		--file /dev/stdin);\
		clear; \
		exec $(USERSHELL)

.PHONY: check-dev
CHECK += check-dev
HELP_check-dev = checks if device is specified
check-dev:
	@if [ "$(DEV)" == "none" ]; \
	then \
		( \
		echo "##############################"; \
		echo "# FLASH DEVICE NOT SPECIFIED #"; \
		echo "##############################"; \
		echo;\
		echo "specify device by adding DEV as parameter";\
		echo;\
		echo "make $(MAKECMDGOALS) DEV=/path/to/device";\
		echo;\
		echo "or by running";\
		echo;\
		echo "make dev";\
		echo;\
		) >&2;\
		exit 1;\
	fi
	@if [ ! -c $(DEV) ]; \
	then \
		( \
		echo "#############################"; \
		echo "# FLASH DEVICE IS NOT VALID #"; \
		echo "#############################"; \
		echo; \
		echo "the specified device ($(DEV)) is not a character device!";\
		echo;\
		echo "specify device by adding DEV as parameter";\
		echo;\
		echo "make $(MAKECMDGOALS) DEV=/path/to/device";\
		echo;\
		echo "or by running";\
		echo;\
		echo "make dev";\
		echo;\
		) >&2;\
		exit 1;\
	fi

### docker targets ###

.PHONY: check-docker
CHECK += check-docker
HELP_check-docker = checks if docker is usable
check-docker:
	@make --no-print-directory -C docker check

### git targets

.PHONY: setup-git
SETUP += setup-git
HELP_setup-git = setup git commit message
setup-git:
	git config --replace-all commit.template .gitcommitmsg

include .Makefile.template
