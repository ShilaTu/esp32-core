
DOCKER_IMAGE := idf/v4.0

# load DEVICE configuration
-include .setup-device.mk
# delete DEVICE configuration if invalid
ifeq ($(shell ! test -c $(DEVICE); echo $$?),0)
$(shell rm -f .setup-device.mk)
undefine DEVICE
else
export DEVICE
endif

DOCKER_VARS        += DEVICE
DOCKER_VARS_DEVICE := $(DEVICE)

DOCKER_VARS += ENVS
DOCKER_VARS_ENVS  := "IDF_PORT=$(DEVICE)"

IDF = idf.py --ccache

VARIABLES += DOCKEROPTIONS
HELP_DOCKEROPTIONS := Docker options

VARIABLES_DOCKEROPTIONS += DEVICE
HELP_DEVICE := specifies which device to pass to docker

TARGETS_all += build
TARGETS_all += test

.PHONY: menuconfig
TARGETS += menuconfig
HELP_menuconfig = configure project
DOCKER += menuconfig
menuconfig:
	$(IDF) menuconfig

.PHONY: reconfigure
TARGET += reconfigure
HELP_reconfigure = reconfigure project (rebuilds cmake files)
DOCKER += reconfigure
reconfigure:
	$(IDF) reconfigure

.PHONY: build-firmware
TARGETS_build += build-firmware
HELP_build-firmware = build firmware files
DOCKER += build-firmware
build-firmware:
	$(IDF) all

.PHONY: clean-build
TARGETS_clean += clean-build
HELP_clean-build = remove generated files of project components
clean-build:
	rm -rf build/esp-idf/

.PHONY: distclean-build
TARGETS_distclean += distclean-build
HELP_distclean-build = remove all generated files
distclean-build:
	rm -f  sdkconfig
	rm -rf build
	rm -rf .setup-device.mk

.PHONY: flash
TARGETS += flash
HELP_flash = flash project to esp. Use DEVICE=path to provide path to device or use make setup
DOCKER += flash
flash: | check-device
	$(IDF) flash -p '$(DEVICE)'

.PHONY: monitor
TARGETS += monitor
HELP_monitor = connect to esp32 via serial. Use DEVICE=path to provide path to device or use make dev
DOCKER += monitor
monitor: | check-device
	$(IDF) monitor -p '$(DEVICE)'

.PHONY: check-device
TARGETS_check += check-device
HELP_check-device = check if valid device is specified
NO_DOCKER += check-device
check-device:
ifndef DEVICE
	@1>&2 echo "##############################"
	@1>&2 echo "# FLASH DEVICE NOT SPECIFIED #"
	@1>&2 echo "##############################"
	@1>&2 echo "specify device by adding DEVICE as parameter"
	@1>&2 echo "  make $(MAKECMDGOALS) DEVICE=/path/to/device"
	@1>&2 echo "or by running"
	@1>&2 echo "  make setup"
	@exit 1
endif
ifeq ($(shell ! test -c $(DEVICE); echo $$?),0)
	@1>&2 echo "#############################"
	@1>&2 echo "# FLASH DEVICE IS NOT VALID #"
	@1>&2 echo "#############################"
	@1>&2 echo "the specified device '$(DEVICE)' is not a character device!"
	@1>&2 echo "specify device by adding DEVICE as parameter"
	@1>&2 echo "  make $(MAKECMDGOALS) DEVICE=/path/to/device"
	@1>&2 echo "or by running"
	@1>&2 echo "  make setup"
	@exit 1
endif
	@exit 0

.PHONY: check-dialog
TARGETS_check += check-dialog
HELP_check-dialog = check if dialog is installed
NO_DOCKER += check-dialog
check-dialog:
ifeq ($(shell which dialog 2> /dev/null),)
	@1>&2 echo "###########################"
	@1>&2 echo "# DIALOG IS NOT INSTALLED #"
	@1>&2 echo "###########################"
	@1>&2 echo "Consider installing dialog."
	@1>&2 echo "on debian based systems:"
	@1>&2 echo "    sudo apt-get install dialog"
	@1>&2 echo "on arch based systems:"
	@1>&2 echo "    sudo pacman -S dialog"
	@exit 1
endif
	@exit 0

.PHONY: setup-device
TARGETS_setup += setup-device
HELP_setup-device = sets device
NO_DOCKER += setup-device
setup-device: | check-dialog
	echo -n DEVICE= > .setup-device.mk
	for dev in $$(ls /dev/serial/by-id); do echo "$$(readlink -f /dev/serial/by-id/$$dev) $$dev "; done \
	| dialog \
		--stdout \
		--menu "choose which device to use" 0 0 0 "none" "disable device passthrough"\
		--file /dev/stdin\
	>> .setup-device.mk || rm -f .setup-device.mk

include $(shell git rev-parse --show-toplevel)/.make/part.mk