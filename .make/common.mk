.DEFAULT_GOAL := help
HELP          ?= LifeSensor Makefile

VARIABLES     += MAKEFILE
HELP_MAKEFILE := Makefile options

VARIABLES_MAKEFILE += DEPENDS
HELP_DEPENDS       ?= dependencies that should be run prior to this Makefile
DEPENDS            ?= #empty

VARIABLES_MAKEFILE += DOCKER
HELP_DOCKER        := run target inside docker (disable with 'DOCKER=')
DOCKER             ?= #empty

VARIABLES_MAKEFILE += DOCKER_IMAGE
HELP_DOCKER_IMAGE  := docker image to use for targets
DOCKER_IMAGE       ?= #empty

VARIABLES_MAKEFILE += DOCKER_VARS
HELP_DOCKER_VARS   := variables to pass to docker Makefile when running docker as shell
DOCKER_VARS        ?= #empty

ifeq ($(find OPTIONS, $(DOCKER_VARS)),)
DOCKER_VARS         += OPTIONS
DOCKER_VARS_OPTIONS ?= #empty
# for github actions set to
# DOCKER_VARS_OPTIONS := --tty=false
endif

ifeq ($(find WORKDIR, $(DOCKER_VARS)),)
DOCKER_VARS         += WORKDIR
DOCKER_VARS_WORKDIR := $(shell pwd)
endif

ifeq ($(find MOUNT, $(DOCKER_VARS)),)
DOCKER_VARS         += MOUNT
DOCKER_VARS_MOUNT   := $(shell git rev-parse --show-toplevel)
endif

TARGETS_common += all
TARGETS_common += build
TARGETS_common += test
TARGETS_common += check
TARGETS_common += setup
TARGETS_common += clean
TARGETS_common += distclean

define TEMPLATE_DEPEND_TARGET
.PHONY: $3
TARGETS_$2 := $3 $(TARGETS_$2)
HELP_$3 := run 'make -C $1 $4'
NO_DOCKER += $3
$3:
	$(MAKE) -C $1 $4
endef

$(foreach \
	SUBDIR, \
	$(DEPENDS), \
	$(foreach \
		TARGET, \
		$(TARGETS_common), \
		$(eval $(call TEMPLATE_DEPEND_TARGET,$(SUBDIR),$(TARGET),$(TARGET)-$(SUBDIR),$(TARGET))) \
	) \
)

define TEMPLATE_COMMON_TARGET
.PHONY: $1
HELP_$1  ?= run common $1 jobs ($2)
NO_DOCKER += $1
$1: $(TARGETS_$1)
endef

TARGETS_distclean := clean $(TARGETS_distclean)
HELP_all := run common jobs ($(TARGETS_all))
$(foreach \
	TARGET, \
	$(TARGETS_common), \
	$(eval $(call TEMPLATE_COMMON_TARGET,$(TARGET),$(TARGETS_$(TARGET)))) \
)

.PHONY: docker
ifdef DOCKER_IMAGE
TARGETS     += docker
HELP_docker := enter docker container
docker:
	$(MAKE) -C $(shell git rev-parse --show-toplevel)/docker/$(DOCKER_IMAGE) run \
	WORKDIR=$(shell pwd) \
	MOUNT=$(shell git rev-parse --show-toplevel) \
	EXEC=bash
else
docker: ;
endif

define TEMPLATE_DOCKER_TARGET
$(if $(DOCKER_$1),,$(if $(DOCKER_IMAGE), $(eval DOCKER_$1 := $(DOCKER_IMAGE)),$(error TARGET '$1' MISSES DOCKER IMAGE!. SET 'DOCKER_$1 = <IMAGE>')))
DOCKER_IMAGES += $(DOCKER_$1)
$1: | docker-$(DOCKER_$1) docker-info
$1: SHELL := $(shell $(MAKE) -sC $$(git rev-parse --show-toplevel)/docker/$(DOCKER_$1) shell $(foreach VAR,$(DOCKER_VARS),$(VAR)=$(DOCKER_VARS_$(VAR))) | grep "docker run" )
endef

$(foreach \
	TARGET, \
	$(DOCKER), \
	$(eval $(call TEMPLATE_DOCKER_TARGET,$(TARGET))) \
)

define TEMPLATE_DOCKER_IMAGE
docker-$1: SHELL := /bin/bash
docker-$1:
	$(MAKE) -C $(shell git rev-parse --show-toplevel)/docker/$1 build
endef

.PHONY: docker-info
docker-info:
	$(info RUNNING IN DOCKER: $(SHELL))

$(foreach \
	IMAGE, \
	$(sort $(DOCKER_IMAGES)), \
	$(eval $(call TEMPLATE_DOCKER_IMAGE,$(IMAGE))) \
)

define TEMPLATE_NODOCKER_TARGET
$1: SHELL := /bin/bash
endef

$(foreach \
	TARGET, \
	$(sort $(NO_DOCKER)), \
	$(eval $(call TEMPLATE_NODOCKER_TARGET,$(TARGET))) \
)

TARGETS := $(TARGETS_common) $(TARGETS)
include $(shell git rev-parse --show-toplevel)/.make/help.mk
