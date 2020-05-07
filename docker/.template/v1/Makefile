### DOCKER IMAGE DEPENDENCY ###
FROM    := base/v1

### DOCKER IMAGE ARGUMENTS ###
ARGS     += FOO
HELP_FOO := modifies something
FOO      := default_value

ARGS     += BAR
HELP_BAR := modifies something else
BAR      := default_value

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