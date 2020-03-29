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

### menuconfig targets ###

.PHONY: menuconfig
TARGET += menuconfig
HELP_menuconfig = configures project
menuconfig: | check-docker
	@make --no-print-directory -C docker idf EXEC="idf.py menuconfig"

.PHONY: menuconfig-test
TARGET += menuconfig-test
HELP_menuconfig-test = configures test project
menuconfig-test: | check-docker
	@make --no-print-directory -C docker idf EXEC="cd test; idf.py menuconfig"

### build targets ###

.PHONY: build
TARGET += build
DEFAULT += build
ALL += build
HELP_build = builds project
build: | check-docker
	@make --no-print-directory -C docker idf EXEC="idf.py build"

.PHONY: clean-build
CLEAN += clean-build
HELP_clean-build = let idf clean generated files
clean-build: | check-docker
	@make --no-print-directory -C docker idf EXEC="idf.py clean"

.PHONY: distclean-build
DISTCLEAN += distclean-build
HELP_distclean-build = removes all generated files
distclean-build:
	rm -f  sdkconfig
	rm -rf build

### test targets ###

.PHONY: test
TARGET += test
HELP_test = build test project, flash it, monitor it
test: build-test flash-test monitor-test

.PHONY: build-test
TARGET_build += build-test
HELP_build-test = builds tests
build-test: | check-docker
	@make --no-print-directory -C docker idf EXEC="cd test; idf.py build"

.PHONY: clean-test
CLEAN += clean-test
HELP_clean-test: let idf clean generated files of the test build
clean-test: | check-docker
	@make --no-print-directory -C docker idf EXEC="cd test; idf.py clean"

.PHONY: distclean-test
DISTCLEAN += distclean-test
HELP_distclean-test = removes all generated files of the test build
distclean-test:
	rm -f  test/sdkconfig
	rm -rf test/build

### flash targets ###

.PHONY: flash
TARGET += flash
HELP_flash = flashes project to esp. Use DEV=path to provide  path to device or use make dev
flash: | check-flash
	@make --no-print-directory -C docker \
		idf \
		EXEC="sudo chgrp developer $(DEV); idf.py flash -p '$(DEV)'"

.PHONY: flash-test
TARGET_flash += flash-test
HELP_flash-test = flashes tests to esp. Use DEV=path to provide  path to device or use make dev
flash-test: | check-flash
	@make --no-print-directory -C docker \
		idf \
		EXEC="sudo chgrp developer $(DEV); cd test; idf.py flash -p '$(DEV)'"

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
		idf \
		EXEC="sudo chgrp developer $(DEV); idf.py monitor -p '$(DEV)'"

.PHONY: monitor-test
TARGET += monitor-test
HELP_monitor-test = connects to esp32 via serial. Use DEV=path to provide path to device or use make dev
monitor-test: | check-monitor
	@make --no-print-directory -C docker \
		idf \
		EXEC="sudo chgrp developer $(DEV); cd test; idf.py monitor -p '$(DEV)'"

.PHONY: check-monitor
CHECK += check-monitor
HELP_check-monitor = checks env if monitor is possible
check-monitor: | check-docker check-dev

### dev targets ###

.PHONY: dev
TARGET += dev
HELP_dev = specifies which USB device to connect to
dev: | check-dialog
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
	@1>&2 echo -n "checking if device is valid & specified..."
ifeq ($(DEV),none)
	@1>&2 echo
	@1>&2 echo "##############################"
	@1>&2 echo "# FLASH DEVICE NOT SPECIFIED #"
	@1>&2 echo "##############################"
	@1>&2 echo
	@1>&2 echo "specify device by adding DEV as parameter"
	@1>&2 echo
	@1>&2 echo "make $(MAKECMDGOALS) DEV=/path/to/device"
	@1>&2 echo
	@1>&2 echo "or by running"
	@1>&2 echo
	@1>&2 echo "make dev"
	@1>&2 echo
	@exit 1
endif
ifeq ($(shell ! test -c $(DEV); echo $$?),0)
	@1>&2 echo
	@1>&2 echo "#############################"
	@1>&2 echo "# FLASH DEVICE IS NOT VALID #"
	@1>&2 echo "#############################"
	@1>&2 echo
	@1>&2 echo "the specified device ($(DEV)) is not a character device!"
	@1>&2 echo
	@1>&2 echo "specify device by adding DEV as parameter"
	@1>&2 echo
	@1>&2 echo "make $(MAKECMDGOALS) DEV=/path/to/device"
	@1>&2 echo
	@1>&2 echo "or by running"
	@1>&2 echo
	@1>&2 echo "make dev"
	@1>&2 echo
	@exit 1
endif
	@1>&2 echo "SUCCESS"

.PHONY: check-dialog
CHECK += check-dialog
HELP_check-dialog = checks if dialog is installed
check-dialog:
	@1>&2 echo -n "checking if dialog is installed..."
ifeq ($(shell which dialog 2> /dev/null),)
	@1>&2 echo
	@1>&2 echo "###########################"
	@1>&2 echo "# DIALOG IS NOT INSTALLED #"
	@1>&2 echo "###########################"
	@1>&2 echo
	@1>&2 echo "Consider installing dialog."
	@1>&2 echo
	@1>&2 echo "on debian based systems:"
	@1>&2 echo "    sudo apt-get install dialog"
	@1>&2 echo
	@1>&2 echo "on arch based systems:"
	@1>&2 echo "    sudo pacman -S dialog"
	@1>&2 echo
	@exit 1
endif
	@1>&2 echo "SUCCESS"

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
	git config --replace-all pull.rebase true

include .Makefile.template
