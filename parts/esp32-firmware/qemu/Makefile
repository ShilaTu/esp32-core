HELP = LifeSensor esp32 qemu firmware
DEPENDS = $(dir $(wildcard components/*/Makefile))

IDF = idf.py --ccache

DOCKER_IMAGE := qemu/esp-develop

TARGETS_all += qemu
TARGETS_all += test

.PHONY: qemu
TARGETS += qemu
HELP_qemu = start qemu
DOCKER += qemu
qemu: build/qemu.img
	qemu-system-xtensa \
		-no-reboot \
		-nographic \
		-machine esp32 \
		-drive file=$^,if=mtd,format=raw \
	2>&1 | tee build/qemu.log

build/qemu.img: build-firmware

.PHONY: build-firmware
TARGETS_build += build-firmware
HELP_build-firmware = build image runnable by qemu
DOCKER += build-firmware
DOCKER_build-firmware = idf/v4.0
build-firmware:
	$(IDF) all
	dd if=/dev/zero bs=1024 count=4096 of=build/qemu.img; \
	dd if=build/bootloader/bootloader.bin bs=1 seek=$$((0x1000)) of=build/qemu.img conv=notrunc; \
	dd if=build/partition_table/partition-table.bin bs=1 seek=$$((0x8000)) of=build/qemu.img conv=notrunc; \
	dd if=$$(ls build/*.bin) bs=1 seek=$$((0x10000)) of=build/qemu.img conv=notrunc;

.PHONY: qemu-gdb
TARGETS += qemu-gdb
HELP_qemu-gdb = start qemu and wait for gdb
DOCKER += qemu-gdb
qemu-gdb: build/qemu.img
	qemu-system-xtensa \
		-s -S \
		-no-reboot \
		-nographic \
		-machine esp32 \
		-drive file=$^,if=mtd,format=raw"

.PHONY: gdb
TARGETS += gdb
HELP_gdb = start gdb and connect to qemu
DOCKER += gdb
gdb:
	xtensa-esp32-elf-gdb $$(ls build/*.elf) \
		-ex \"target remote :1234\" \
		-ex \"tb app_main\" \
		-ex \"c\""

build/qemu.log: qemu

.PHONY: test-qemu
TARGETS_test += test-qemu
HELP_test-qemu = check errors in qemu run
test-qemu: build/qemu.log
	grep -q "0 Failures" $^


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

include $(shell git rev-parse --show-toplevel)/.make/common.mk