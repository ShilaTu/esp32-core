HELP := LifeSensor product

TARGET_default := all
TARGET_all     := build test

VARIABLE += PARTS
HELP_PARTS := parts this product uses

.phony: build-requirements
TARGET_build += build-requirements
HELP_build-requirements := build requirements documents
build-requirements:
	#todo

.phony: test-requirements
TARGET_test += test-requirements
HELP_test-requirements := test if requirements are fullfilled
test-requirements:
	#todo

.phony: clean-requirements
TARGET_clean += clean-requirements
HELP_clean-requirements := clean generated requirement files
clean-requirements:
	#todo

define PART_TEMPLATE
.phony: $1-parts
TARGET_$1 += $1-parts
HELP_$1-parts := run make $1 for all parts
$1-parts:
	$(foreach PART,$(PARTS),make -C ../../../parts/$(PART) $1)
endef

$(foreach TARGET, build test clean distclean, $(eval $(call PART_TEMPLATE,$(TARGET))))

include $(shell git rev-parse --show-toplevel)/.make/common.mk