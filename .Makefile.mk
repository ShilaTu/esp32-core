#
# see .Makefile.template for help
#

### variables ###

VARIABLE+=ESPPORT
HELP_ESPPORT=which device to flash/monitor
export ESPPORT

### internal variables ###

# assume /etc/passwd holds right shell of user
USERSHELL=$(shell awk -F ':' '/^'$$(id -un)':/{print $$NF}' /etc/passwd)

# export any additional docker options for docker Makefile
export DOCKEROPTS

# assume we are in projectdir
PROJECTDIR=$(shell pwd)

# assume dirname is projectname
PROJECT=$(notdir $(shell pwd))

# assume docker Makefile dir
DOCKERDIR=../docker

# assume template path
TEMPLATEPATH=../.Makefile.template

### default target ###
DEFAULT += help

### menuconfig targets ###

.PHONY: menuconfig
TARGET += menuconfig

HELP_menuconfig = configure project
menuconfig: | check-docker
	@make --no-print-directory -C $(DOCKERDIR) idf \
		WORKDIR=$(PROJECTDIR) \
		EXEC="idf.py menuconfig"

### build targets ###

.PHONY: reconfigure
TARGET += reconfigure
ALL += reconfigure
HELP_reconfigure = reconfigure project (rebuilds cmake files)
reconfigure: | check-docker
	@make --no-print-directory -C $(DOCKERDIR) idf \
		WORKDIR=$(PROJECTDIR) \
		EXEC="idf.py reconfigure"

.PHONY: build
TARGET += build
ALL += build
HELP_build = build project
build: | check-docker
	@make --no-print-directory -C $(DOCKERDIR) idf \
		WORKDIR=$(PROJECTDIR) \
		EXEC="idf.py --ccache build"

.PHONY: clean-build
CLEAN += clean-build
HELP_clean-build = remove generated files of project components
clean-build: | check-docker
	rm -rf $(addprefix $(PROJECTDIR)/build/esp-idf/, $(shell find $(PROJECTDIR)/../components -mindepth 1  -maxdepth 1 -type d -printf "%f\n") main)

.PHONY: distclean-build
DISTCLEAN += distclean-build
HELP_distclean-build = remove all generated files
distclean-build:
	rm -f  $(PROJECTDIR)/sdkconfig
	rm -rf $(PROJECTDIR)/build

### flash targets ###

.PHONY: flash
TARGET += flash
HELP_flash = flash project to esp. Use ESPPORT=path to provide  path to device or use make dev
flash: | check-flash
	@make --no-print-directory -C $(DOCKERDIR) idf \
		WORKDIR=$(PROJECTDIR) \
		EXEC="sudo chgrp developer $(ESPPORT);\
		      idf.py flash -p '$(ESPPORT)'"

.PHONY: check-flash
CHECK += check-flash
HELP_check-flash = check env if flashing is possible
check-flash: | check-docker check-dev

### monitor targets ###

.PHONY: monitor
TARGET += monitor
HELP_monitor = connect to esp32 via serial. Use ESPPORT=path to provide path to device or use make dev
monitor: | check-monitor
	@make --no-print-directory -C $(DOCKERDIR) idf \
		WORKDIR=$(PROJECTDIR) \
		EXEC="sudo chgrp developer $(ESPPORT); \
		      idf.py monitor -p '$(ESPPORT)'"

.PHONY: check-monitor
CHECK += check-monitor
HELP_check-monitor = check env if monitor is possible
check-monitor: | check-docker check-dev

### dev targets ###

.PHONY: dev
TARGET += dev
HELP_dev = specify which USB device to connect to
dev: | check-dialog
	export ESPPORT=$$((for dev in $$(ls /dev/serial/by-id); do echo "$$(readlink -f /dev/serial/by-id/$$dev) $$dev "; done ) \
	| dialog \
		--stdout \
		--menu "choose which device to use" 0 0 0 "none" "disable device passthrough"\
		--file /dev/stdin);\
		clear; \
		exec $(USERSHELL)

.PHONY: check-dev
CHECK += check-dev
HELP_check-dev = check if device is specified
check-dev:
	@1>&2 echo -n "checking if device is valid & specified..."
ifndef ESPPORT
	@1>&2 echo
	@1>&2 echo "##############################"
	@1>&2 echo "# FLASH DEVICE NOT SPECIFIED #"
	@1>&2 echo "##############################"
	@1>&2 echo
	@1>&2 echo "specify device by adding ESPPORT as parameter"
	@1>&2 echo
	@1>&2 echo "make $(MAKECMDGOALS) ESPPORT=/path/to/device"
	@1>&2 echo
	@1>&2 echo "or by running"
	@1>&2 echo
	@1>&2 echo "make dev"
	@1>&2 echo
	@exit 1
endif
ifeq ($(shell ! test -c $(ESPPORT); echo $$?),0)
	@1>&2 echo
	@1>&2 echo "#############################"
	@1>&2 echo "# FLASH DEVICE IS NOT VALID #"
	@1>&2 echo "#############################"
	@1>&2 echo
	@1>&2 echo "the specified device ($(ESPPORT)) is not a character device!"
	@1>&2 echo
	@1>&2 echo "specify device by adding ESPPORT as parameter"
	@1>&2 echo
	@1>&2 echo "make $(MAKECMDGOALS) ESPPORT=/path/to/device"
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
HELP_check-dialog = check if dialog is installed
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
HELP_check-docker = check if docker is usable
check-docker:
	@make --no-print-directory -C $(DOCKERDIR) check

### git targets

.PHONY: setup-git
SETUP += setup-git
HELP_setup-git = setup git commit message
setup-git:
	git config --replace-all commit.template .gitcommitmsg
	git config --replace-all pull.rebase true

include $(TEMPLATEPATH)
