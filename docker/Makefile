#
# see ../.Makefile.template for help
#

### variables ###

VARIABLE += IDF_VERSION
HELP_IDF_VERSION = which idf version to use
IDF_VERSION?=v4.0

VARIABLE += QEMU_COMMIT
HELP_QEMU_COMMIT = which qemu commit to checkout
QEMU_COMMIT?=esp-develop

VARIABLE += DEV
HELP_DEV = which device to map into container
DEV?=none

ifneq ("$(DEV)","none")
DOCKER_DEV=--device $(DEV)
endif

VARIABLE += UID
HELP_UID = user id of user inside of container
UID?=$(shell id -u)

VARIABLE += GID
HELP_GID = group id of user inside of container
GID?=$(shell id -g)

VARIABLE += EXEC
HELP_EXEC = what to execute in the container
EXEC?=bash

VARIABLE += DOCKEROPTS
HELP_DOCKEROPTS = additional docker options

### docker build targets ###

.PHONY: idf-image
ALL += idf-image
DEFAULT += idf-image
TARGET_idf += idf-image
HELP_idf-image = builds docker idf image
idf-image: | check-docker
	docker build \
		--build-arg=idf_version=$(IDF_VERSION) \
		--build-arg=UID=$(UID) \
		--build-arg=GID=$(GID) \
		-t "lifesensor/idf:$(IDF_VERSION)" \
		idf

.PHONY: qemu-image
ALL += qemu-image
DEFAULT += qemu-image
TARGET_qemu += qemu-image
HELP_qemu-image = buils docker qemu image
qemu-image: idf-image | check-docker
	docker build \
		--build-arg=idf_version=$(IDF_VERSION) \
		--build-arg=qemu_commit=$(QEMU_COMMIT) \
		-t "lifesensor/qemu:$(QEMU_COMMIT)" \
		qemu

.PHONY: vscode-image
ALL += vscode-image
TARGET_vscode += vscode-image
HELP_vscode-image = buils docker vscode image
vscode-image:  qemu-image | check-docker
	docker build \
		--build-arg=qemu_commit=$(QEMU_COMMIT) \
		-t "lifesensor/vscode:latest" \
		vscode

### docker run targets ###

.PHONY: idf
TARGET += idf 
HELP_idf = runs idf container
idf: | idf-image check-docker
	docker run \
		--rm \
		-ti \
		$(DOCKER_DEV) \
		-v idf-home:/home/developer:rw \
		-v "$(shell pwd)/..:$(shell pwd)/..:rw" \
		-w "$(shell pwd)/.." \
		--hostname idf \
		$(DOCKEROPTS) \
		lifesensor/idf:$(IDF_VERSION) \
		/bin/bash -ic '$(EXEC)'

.PHONY: qemu
TARGET += qemu
HELP_qemu = runs qemu container
qemu: | qemu-image check-docker
	docker run \
		--rm \
		-ti \
		$(DOCKER_DEV) \
		-v qemu-home:/home/developer:rw \
		-v "$(shell pwd)/..:$(shell pwd)/..:rw" \
		-w "$(shell pwd)/.." \
		--hostname qemu \
		$(DOCKEROPTS) \
		lifesensor/qemu:$(QEMU_COMMIT) \
		/bin/sh -c '$(EXEC)'

.PHONY: code vscode
TARGET += vscode
HELP_vscode = runs vscode container
TARGET += code
HELP_code = runs vscode container
code vscode: | vscode-image check-docker
	xhost local:root
	docker run \
		--rm \
		-ti \
		$(DOCKER_DEV) \
		-v vscode-home:/home/developer:rw \
		-v /tmp/.X11-unix/:/tmp/.X11-unix \
		-e DISPLAY=$(DISPLAY) \
		-e LD_PRELOAD=/usr/lib/x86_64-linux-gnu/libglib-2.0.so.0 \
		-v "$(shell pwd)/..:$(shell pwd)/..:rw" \
		-w "$(shell pwd)/.." \
		--shm-size=512m \
		--hostname vscode \
		$(DOCKEROPTS) \
		lifesensor/vscode:latest \
		/bin/sh -c '$(EXEC)'

### docker remove volume targets ###

.PHONY: clean-idf
CLEAN += clean-idf
HELP_clean-idf = removes idf volume
clean-idf:
	-docker volume rm idf-home

.PHONY: clean-qemu
CLEAN += clean-qemu
HELP_clean-qemu = removes qemu volume
clean-qemu:
	-docker volume rm qemu-home

.PHONY: clean-vscode
CLEAN += clean-vscode
HELP_clean-vscode = removes vscode volume
clean-vscode:
	-docker volume rm vscode-home

### docker remove image targets ###

.PHONY: distclean-idf
DISTCLEAN += distclean-idf
HELP_distclean-idf = removes idf docker image
distclean-idf: clean-idf
	-docker image remove "lifesensor/idf:$(IDF_VERSION)"
	docker image prune
	
.PHONY: distclean-qemu
DISTCLEAN += distclean-qemu
HELP_distclean-qemu = removes qemu docker image
distclean-qemu: clean-qemu
	-docker image remove "lifesensor/qemu:$(QEMU_COMMIT)"
	docker image prune

.PHONY: distclean-vscode
DISTCLEAN += distclean-vscode
HELP_distclean-vscode = removes vscode docker image
distclean-vscode: clean-vscode
	-docker image remove "lifesensor/vscode:latest"
	docker image prune

### docker check installation targets ###

.PHONY: check-docker
CHECK += check-docker
check-docker: check-docker-installed check-docker-group

.PHONY: check-docker-installed
check-docker-installed:
ifeq (,$(shell which docker 2> /dev/null))
	@1>&2 echo "########################################"
	@1>&2 echo "# DOCKER DOES NOT SEEM TO BE INSTALLED #"
	@1>&2 echo "########################################"
	@1>&2 echo "the docker binary could not be found in your PATH!"
	@1>&2 echo "This usally means docker is simply not installed."
	@1>&2 echo
ifneq (,$(shell grep -i debian /etc/*-release))
	@1>&2 echo "I detected your platform as DEBIAN, so please run"
	@1>&2 echo
	@1>&2 make -sn install-docker-debian
	@1>&2 echo
	@1>&2 echo OR simply
	@1>&2 echo
	@1>&2 echo make install-docker-debian
	@1>&2 echo
else ifneq (,$(shell grep -i ubuntu /etc/*-release))
	@1>&2 echo "I detected your platform as UBUNTU, so please run"
	@1>&2 echo
	@1>&2 make -sn install-docker-ubuntu
	@1>&2 echo
	@1>&2 echo OR simply
	@1>&2 echo
	@1>&2 echo make install-docker-ubuntu
	@1>&2 echo
else ifneq (,$(shell grep -i "arch linux" /etc/*-release))
	@1>&2 echo "I detected your platform as ARCHLINUX, so please run"
	@1>&2 echo
	@1>&2 make -sn install-docker-archlinux
	@1>&2 echo
	@1>&2 echo OR simply
	@1>&2 echo
	@1>&2 echo make install-docker-archlinux
	@1>&2 echo
else ifneq (,$(shell grep -i centos /etc/*-release))
	@1>&2 echo "I detected your platform as CENTOS, so please run"
	@1>&2 echo
	@1>&2 make -sn install-docker-centos
	@1>&2 echo
	@1>&2 echo OR simply
	@1>&2 echo
	@1>&2 echo make install-docker-centos
	@1>&2 echo
else ifneq (,$(shell grep -i fedora /etc/*-release))
	@1>&2 echo "I detected your platform as FEDORA, so please run"
	@1>&2 echo
	@1>&2 make -sn install-docker-fedora
	@1>&2 echo
	@1>&2 echo OR simply
	@1>&2 echo
	@1>&2 echo make install-docker-fedora
	@1>&2 echo
else
	@1>&2 echo "I detected your platform as UNKNOWN,"
	@1>&2 echo
	@1>&2 echo please run your package manager to install docker
endif
	@exit 1
endif

### docker install targets ###

.PHONY: install-docker-debian
install-docker-debian:
	sudo apt-get update
	sudo apt-get install apt-transport-https ca-certificates curl gnupg2 software-properties-common
	curl -fsSL https://download.docker.com/linux/debian/gpg | sudo apt-key add -
	sudo add-apt-repository "deb [arch=amd64] https://download.docker.com/linux/debian $$(lsb_release -cs) stable"
	sudo apt-get update
	sudo apt-get install docker-ce docker-ce-cli containerd.io

.PHONY: install-docker-ubuntu
install-docker-ubuntu:
	sudo apt-get update
	sudo apt-get install apt-transport-https ca-certificates curl gnupg-agent software-properties-common
	curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -
	sudo add-apt-repository "deb [arch=amd64] https://download.docker.com/linux/ubuntu $$(lsb_release -cs) stable"
	sudo apt-get install docker-ce docker-ce-cli containerd.io
	sudo apt-get update

.PHONY: install-docker-archlinux
install-docker-archlinux:
	sudo pacman -Sy docker

.PHONY: install-docker-centos
install-docker-centos:
	sudo yum install -y yum-utils device-mapper-persistent-data lvm2
	sudo yum-config-manager --add-repo https://download.docker.com/linux/centos/docker-ce.repo
	sudo yum install docker-ce docker-ce-cli containerd.io

.PHONY: install-docker-fedora
install-docker-fedora:
	sudo dnf -y install dnf-plugins-core
	sudo dnf config-manager --add-repo https://download.docker.com/linux/fedora/docker-ce.repo
	sudo dnf install docker-ce docker-ce-cli containerd.io

### docker check group target ###

.PHONY: check-docker-group
check-docker-group:
ifneq (,$(shell groups | grep -q docker))
	@1>&2 echo "##############################################"
	@1>&2 echo "# YOUR USER IS NOT PART OF THE DOCKER GROUP! #"
	@1>&2 echo "##############################################"
	@1>&2 echo "This means you need to execute every docker command with sudo"
	@1>&2 echo "To avoid this, please add your user ($$USER) to the docker group with the following command:"
	@1>&2 echo
	@1>&2 echo "sudo usermod -a -G docker $$USER"
	@1>&2 echo
	@exit 1
endif

include ../.Makefile.template
