DEPENDS := ../../idf/v4.0

### DOCKER IMAGE ARGUMENTS ###
ARGS              += QEMU_VERSION
HELP_QEMU_VERSION := qemu version to use
QEMU_VERSION      := esp-develop

ARGS        += IDF_VERSION
IDF_VERSION := v4.0

### DOCKER BUILD OPTIONS ###
TAG     := latest
BUILD   := #empty

### DOCKER RUN OPTIONS ###
RUN     := #empty
VOLUMES := lifesensor-qemu-esp-develop-home:/home/developer:rw
WORKDIR := $(shell pwd)
ENVS    := #empty
EXEC    := #empty
MOUNT   := $(shell pwd)

include $(shell git rev-parse --show-toplevel)/.make/docker.mk