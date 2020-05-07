### DOCKER IMAGE DEPENDENCY ###
FROM    := #empty

### DOCKER IMAGE ARGUMENTS ###
ARGS     += UID
HELP_UID := user id of developer
UID      := $(shell id -u)

ARGS     += GID
HELP_GID := group id of developer
GID      := $(shell id -g)

### DOCKER BUILD OPTIONS ###
TAG     := latest
BUILD   := #empty

### DOCKER RUN OPTIONS ###
RUN     := #empty
VOLUMES := #empty
WORKDIR := $(shell pwd)
ENVS    := #empty
EXEC    := #empty
MOUNT   := $(shell pwd)

include $(shell git rev-parse --show-toplevel)/.make/docker.mk