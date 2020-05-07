DEPENDS := ../../base/v1

### DOCKER IMAGE ARGUMENTS ###
ARGS             += IDF_VERSION
HELP_IDF_VERSION := idf version to use
IDF_VERSION      := v4.0

### DOCKER BUILD OPTIONS ###
TAG     := latest
BUILD   := #empty

### DOCKER RUN OPTIONS ###
RUN     := #empty
VOLUMES += lifesensor-idf-v4.0-home-ccache:/home/developer/.ccache:rw
WORKDIR := $(shell pwd)
ENVS    := #empty
EXEC    := #empty
MOUNT   := $(shell pwd)

include $(shell git rev-parse --show-toplevel)/.make/docker.mk