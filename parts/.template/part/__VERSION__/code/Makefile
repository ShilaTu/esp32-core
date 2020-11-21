
TARGET_default := all
TARGET_all     := build-firmware

firmware: main.c
	gcc -o $@ $<

.phony: build-firmware
TARGET_build += build-firmware
build-firmware: firmware

.phony: clean-firmware
TARGET_clean += clean-firmware
clean-firmware:
	rm -f firmware

include $(shell git rev-parse --show-toplevel)/.make/common.mk