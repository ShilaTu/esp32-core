
VARIABLE+=DEV
HELP_DEV=which device to flash/monitor
-include .dev
DEV?=none

.DEFAULT_GOAL=default

.PHONY: help
help:
	@echo "--- help ---"
	@[ -n "$(HELP_$(.DEFAULT_GOAL))" ] && echo -e "make\n\t$(HELP_$(.DEFAULT_GOAL))" || true;
	@$(foreach \
		target, \
		$(TARGET), \
		echo "make $(target)"; \
		[ -n "$(HELP_$(target))" ] && echo -e "\t$(HELP_$(target))"; \
	)
	@$(foreach \
		target, \
		$(TARGET), \
		[ -n "$(TARGET_$(target))" ] && echo -e "\n--- $(target) sub targets ---"  || true; \
		$(foreach \
			subtarget, \
			$(TARGET_$(target)), \
				echo "make $(subtarget)"; \
				[ -n "$(HELP_$(subtarget))" ] && echo -e "\t$(HELP_$(subtarget))" || true; \
		) \
	)
	@[ -n "$(VARIABLE)" ] && echo -e "\n--- VARIABLES ---"  || true;
	@$(foreach \
		variable, \
		$(VARIABLE), \
		echo "$(variable)"; \
		[ -n "$(HELP_$(variable))" ] && echo -e "\t$(HELP_$(variable))" || true; \
		echo -e "\t(currently: $($(variable)))"; \
	)


TARGET += default
HELP_default  = builds project
.PHONY: default
default: build

TARGET += all
HELP_all = builds, flashes and starts monitor
.PHONY: all
all: build flash monitor

TARGET += setup
HELP_setup = sets up project environment
.PHONY: setup
setup: setup-git

TARGET += check
HELP_check = checks if everything is correctly setup
.PHONY: check
check: check-build check-flash check-monitor

TARGET += clean
HELP_clean = cleans up project
.PHONY: clean
clean: clean-build clean-flash clean-monitor clean-dev
	@make --no-print-directory -C docker clean

TARGET += distclean
HELP_distclean = removes all generated files
.PHONY: distclean
distclean: clean distclean-build distclean-flash distclean-monitor distclean-dev
	@make --no-print-directory -C docker distclean

### build targets ###

TARGET += build
HELP_build = builds project
.PHONY: build
build: | check-build
	@make --no-print-directory -C docker pio EXEC="pio run"

.PHONY: check-build
check-build: check-docker

.PHONY: clean-build
clean-build:
	@make --no-print-directory -C docker pio EXEC="pio run -t clean"

.PHONY: distclean-build
distclean-build: clean-build
	rm -rf .pio

### flash targets ###

TARGET += flash
HELP_flash = flashes project to esp. Use DEV=path to provide  path to device or use make dev
.PHONY: flash
flash: | check-flash
	@make --no-print-directory -C docker \
		pio \
		EXEC="sudo chgrp developer $(DEV); pio run --upload-port '$(DEV)' -t upload" DEV="$(DEV)"

.PHONY: check-flash
check-flash: | check-docker check-dev

.PHONY: clean-flash
clean-flash:
	@true

.PHONY: distclean-flash
distclean-flash: clean-flash
	@true

### monitor targets ###

TARGET += monitor
HELP_monitor = connects to esp32 via serial. Use DEV=path to provide path to device or use make dev
.PHONY: monitor
monitor: | check-monitor
	@make --no-print-directory -C docker \
		pio \
		EXEC="sudo chgrp developer $(DEV); \
			python3 \
				/home/developer/.platformio/packages/framework-espidf/tools/idf_monitor.py \
				/mnt/.pio/build/pico32/firmware.elf \
				--port $(DEV)" \
		DEV=$(DEV)

TARGET_monitor += check-monitor
HELP_check-monitor = checks if monitor is usable
.PHONY: check-monitor
check-monitor: | check-docker check-dev

.PHONY: clean-monitor
clean-monitor:
	@true

.PHONY: distclean-monitor
distclean-monitor: clean-monitor
	@true

### dev targets ###

TARGET += dev
HELP_dev = specifies which USB device to connect to
.PHONY: dev
dev:
	echo -n "DEV=" > ./.dev; \
	(for dev in $$(ls /dev/serial/by-id); do echo "$$(readlink -f /dev/serial/by-id/$$dev) $$dev "; done ) \
	| dialog \
		--stdout \
		--menu "choose which device to flash" 0 0 0 \
		--file /dev/stdin \
	>> ./.dev
	clear

TARGET_dev += check-dev
HELP_check-dev = checks if device is specified
.PHONY: check-dev
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

TARGET_dev += clean-dev
HELP_clean-dev = resets specified device
.PHONY: clean-dev
clean-dev:
	rm -f .dev

.PHONY: distclean-dev
distclean-dev: clean-dev

### misc targets ###
TARGET += check-docker
HELP_check-docker = checks if docker is installed
.PHONY: check-docker
check-docker:
	@make --no-print-directory -C docker check

### git targets

SETUP += setup-git
.PHONY: setup-git
setup-git:
	git config --replace-all commit.template .gitcommitmsg
