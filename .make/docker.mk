HELP := LifeSensor docker image & container

COMMIT := $(shell git rev-list --all --abbrev-commit -1 -- $(realpath $(firstword $(MAKEFILE_LIST))))
ifndef COMMIT
COMMIT := unknown
$(warning COMMIT OF $(realpath $(firstword $(MAKEFILE_LIST))) UNDEFINED!)
endif

VERSION := $(shell basename $$(dirname $(realpath $(firstword $(MAKEFILE_LIST)))))
ifndef VERSION
VERSION := unknown
$(warning VERSION OF $(realpath $(firstword $(MAKEFILE_LIST))) UNDEFINED!)
endif

NAME := $(shell basename $$(dirname $$(dirname $(realpath $(firstword $(MAKEFILE_LIST))))))
ifndef NAME
NAME := unknown
$(error NAME OF $(realpath $(firstword $(MAKEFILE_LIST))) UNDEFINED!)
endif

VARIABLES         += DOCKER_IMAGE
HELP_DOCKER_IMAGE := docker image options

VARIABLES_DOCKER_IMAGE += IMAGE
HELP_IMAGE             := name of the docker image
IMAGE                  := lifesensor/$(NAME)/$(VERSION)

VARIABLES_DOCKER_IMAGE += TAG
HELP_TAG               := tag of the docker image
TAG                    ?= latest


VARIABLES         += DOCKER_BUILD
HELP_DOCKER_BUILD := docker build options

VARIABLES_DOCKER_BUILD += $(ARGS)

VARIABLES_DOCKER_BUILD += BUILD
HELP_BUILD             := additional docker build options
BUILD                  ?= #empty

VARIABLES       += DOCKER_RUN
HELP_DOCKER_RUN := docker run options

VARIABLES_DOCKER_RUN += OPTIONS
HELP_OPTIONS         := additional docker run options
OPTIONS              ?= #empty

VARIABLES_DOCKER_RUN += VOLUMES
HELP_VOLUMES         := volumes to mount inside container
VOLUMES              ?= #empty

VARIABLES_DOCKER_RUN += WORKDIR
HELP_WORKDIR         := where to start in docker volume
WORKDIR              ?= #empty

VARIABLES_DOCKER_RUN += ENVS
HELP_ENVS            := additional environment variables to set
ENVS                 ?= #empty

VARIABLES_DOCKER_RUN += EXEC
HELP_EXEC            := additional environment variables to set
EXEC                 ?= /bin/bash

VARIABLES_DOCKER_RUN += MOUNT
HELP_MOUNT           := what directory to mount into the container
MOUNT                ?= #empty

VARIABLES_DOCKER_RUN += DEVICE
HELP_DEVICE          := what device to pass into the container
DEVICE               ?= #empty

.PHONY: build-image
TARGETS_build    += build-image
HELP_build-image := build docker image '$(IMAGE):$(TAG)'
build-image: | check-docker
	docker build \
		$(foreach ARG,$(ARGS),--build-arg=$(ARG)=$($(ARG)) ) \
		$(BUILD) \
		-t "$(IMAGE):$(COMMIT)" \
		.
	docker image tag "$(IMAGE):$(COMMIT)" "$(IMAGE):$(TAG)"

.PHONY: run
TARGETS  += run
HELP_run := run docker image '$(IMAGE):$(TAG)'
run: build | check-docker
	docker run --rm -ti --hostname $(NAME) --name $(NAME)\
		$(foreach VOLUME, $(VOLUMES),-v "$(VOLUME)" ) \
		$(foreach ENV, $(ENVS),-e "$(ENV)" ) \
		$(if $(WORKDIR),-w "$(WORKDIR)") \
		$(if $(DEVICE),--device "$(DEVICE)") \
		$(if $(MOUNT),-v $(abspath $(MOUNT)):$(abspath $(MOUNT)):rw) \
		$(OPTIONS) \
		"$(IMAGE):$(TAG)" \
		/bin/bash -ic '$(EXEC)'

.PHONY: shell
TARGETS += shell
HELP_shell := prints command used by run without executing anything
shell:
	echo docker run --rm -ti --hostname $(NAME) --name $(NAME)\
		$(foreach VOLUME, $(VOLUMES),-v "$(VOLUME)" ) \
		$(foreach ENV, $(ENVS),-e "$(ENV)" ) \
		$(if $(WORKDIR),-w "$(WORKDIR)") \
		$(if $(DEVICE),--device "$(DEVICE)") \
		$(if $(MOUNT),-v $(abspath $(MOUNT)):$(abspath $(MOUNT)):rw) \
		$(OPTIONS) \
		"$(IMAGE):$(TAG)"\
		/bin/bash -i

.PHONY: exec
TARGETS  += exec
HELP_run := execute command in docker container '$(NAME)'
exec: build | check-docker
	docker exec -ti $(NAME) /bin/bash -ic '$(EXEC)'

.PHONY: clean-volume
TARGETS_clean     += clean-volume
HELP_clean-volume := delete docker volumes
clean-volume: | check-docker
	$(foreach \
		VOLUME, \
		$(shell echo "$(VOLUMES)"| tr ' ' '\n' | grep -o '^[^/][^:]*'), \
		docker volume rm -f $(VOLUME); \
	)

.PHONY: distclean-image
TARGETS_distclean    += distclean-image
HELP_distclean-image := delete docker image
distclean-image: | check-docker
	for parent in $$(docker images $(IMAGE) -q | sort -u); \
	do \
		echo $$parent; \
		for image in $$(docker images -q --filter since=$$parent);\
		do \
			docker history $$image | grep -q $$parent && echo $$image; \
		done; \
	done | sort -u | xargs -r docker image rm -f 

.PHONY: check-docker
check-docker:
ifeq ($(shell which docker 2> /dev/null),)
	@1>&2 echo "########################################"
	@1>&2 echo "# DOCKER DOES NOT SEEM TO BE INSTALLED #"
	@1>&2 echo "########################################"
	@1>&2 echo "the docker binary could not be found in your PATH!"
	@1>&2 echo "This usally means docker is simply not installed."
	@exit 1
endif
ifneq ($(shell groups | grep -q docker),)
	@1>&2 echo "##############################################"
	@1>&2 echo "# YOUR USER IS NOT PART OF THE DOCKER GROUP! #"
	@1>&2 echo "##############################################"
	@1>&2 echo "This means you need to execute every docker command with sudo"
	@1>&2 echo "To avoid this, please add your user ($$USER) to the docker group with the following command:"
	@1>&2 echo "    sudo usermod -a -G docker $$USER"
	@1>&2 echo "you may need to login again to apply the settings"
	@exit 1
endif

include $(shell git rev-parse --show-toplevel)/.make/common.mk