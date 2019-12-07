VARIABLE += PLATFORMIO_VERSION
HELP_PLATFORMIO_VERSION = which platformio version to use
PLATFORMIO_VERSION?=4.1.0

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

.DEFAULT_GOAL=default

.PHONY: help
help:
	@echo "--- help ---"
	@[ -n "$(HELP_$(.DEFAULT_GOAL))" ] && echo -e "make\n\t$(HELP_$(.DEFAULT_GOAL))" || true;
	@$(foreach \
		target, \
		$(TARGET), \
		echo "make $(target)"; \
		[ -n "$(HELP_$(target))" ] && echo -e "\t$(HELP_$(target)) || true"; \
	)
	@$(foreach \
		target, \
		$(TARGET), \
		[ -n "$(TARGET_$(target))" ] && echo -e "\n--- $(target) sub targets ---"  || true; \
		$(foreach \
			subtarget, \
			$(TARGET_$(target)), \
				echo "make $(subtarget)"; \
				[ -n "$(HELP_$(subtarget))" ] && echo -e "\t$(HELP_$(subtarget))" || true; \
		) \
	)
	@[ -n "$(VARIABLE)" ] && echo -e "\n--- VARIABLES ---"  || true;
	@$(foreach \
		variable, \
		$(VARIABLE), \
		echo "$(variable)"; \
		[ -n "$(HELP_$(variable))" ] && echo -e "\t$(HELP_$(variable))" || true; \
		echo -e "\t(currently: '$($(variable))')"; \
	)

TARGET += default
HELP_default = checks docker, builds platformio/qemu docker image
.PHONY: default
default: check docker-platformio docker-qemu

TARGET += all
HELP_all = checks docker and build all containers
.PHONY: all
all: check docker

TARGET += check
HELP_check = checks if docker is installed
.PHONY: check
check: check-docker

TARGET += docker
HELP_docker = build all docker containers
.PHONY: docker
docker: docker-platformio docker-qemu docker-vscode

TARGET_docker += docker-platformio
HELP_docker-platformio = builds docker platformio image
.PHONY: docker-platformio
docker-platformio: | check-docker
	docker build \
		--build-arg=platformio_version=$(PLATFORMIO_VERSION) \
		--build-arg=UID=$(UID) \
		--build-arg=GID=$(GID) \
		-t "lifesensor/platformio:$(PLATFORMIO_VERSION)" \
		platformio

TARGET_docker += docker-qemu
HELP_docker-qemu = buils docker qemu image
.PHONY: docker-qemu
docker-qemu: docker-platformio | check-docker
	docker build \
		--build-arg=platformio_version=$(PLATFORMIO_VERSION) \
		--build-arg=qemu_commit=$(QEMU_COMMIT) \
		-t "lifesensor/qemu:$(QEMU_COMMIT)" \
		qemu

TARGET_docker += docker-vscode
HELP_docker-vscode = buils docker vscode image
.PHONY: docker-vscode
docker-vscode:  docker-qemu | check-docker
	docker build \
		--build-arg=platformio_version=$(PLATFORMIO_VERSION) \
		--build-arg=qemu_commit=$(QEMU_COMMIT) \
		-t "lifesensor/vscode:latest" \
		vscode
		
TARGET += pio 
HELP_pio = runs platformio container
TARGET += platformio 
HELP_platformio = runs platformio container
.PHONY: pio platformio
pio platformio: | docker-platformio check-docker
	docker run \
		--rm \
		-ti \
		$(DOCKER_DEV) \
		-v pio-home:/home/developer:rw \
		-v "$(shell pwd)/..:$(shell pwd)/..:rw" \
		-w "$(shell pwd)/.." \
		--hostname platformio \
		$(DOCKEROPTS) \
		lifesensor/platformio:$(PLATFORMIO_VERSION) \
		/bin/sh -c '$(EXEC)'

TARGET += qemu
HELP_qemu = runs qemu container
.PHONY: qemu
qemu: | docker-qemu check-docker
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

TARGET += vscode
HELP_vscode = runs vscode container
TARGET += code
HELP_code = runs vscode container
.PHONY: code vscode
code vscode: | docker-vscode check-docker
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

TARGET += clean
HELP_clean = cleans docker volumes
.PHONY: clean
clean: clean-docker

.PHONY: clean-docker
clean-docker: clean-docker-platformio clean-docker-qemu clean-docker-vscode

TARGET_clean += clean-docker-platformio
HELP_clean-docker-platformio = removes platformio volume
.PHONY: clean-docker-platformio
clean-docker-platformio:
	-docker volume rm pio-home

TARGET_clean += clean-docker-qemu
HELP_clean-docker-qemu = removes qemu volume
.PHONY: clean-docker-qemu
clean-docker-qemu:
	-docker volume rm qemu-home

TARGET_clean += clean-docker-vscode
HELP_clean-docker-vscode = removes vscode volume
.PHONY: clean-docker-vscode
clean-docker-vscode:
	-docker volume rm vscode-home

TARGET += distclean
HELP_distclean = removes all generated files by docker
.PHONY: distclean
distclean: clean distclean-check distclean-docker

.PHONY: distclean-docker
distclean-docker: clean-docker distclean-docker-platformio distclean-docker-qemu distclean-docker-vscode
	-docker image prune

TARGET_distclean += distclean-docker-platformio
HELP_distclean-docker-platformio = removes platformio docker image
.PHONY: distclean-docker-platformio
distclean-docker-platformio: clean-docker-platformio
	-docker image remove "lifesensor/platformio:$(PLATFORMIO_VERSION)"
	
TARGET_distclean += distclean-docker-qemu
HELP_distclean-docker-qemu = removes qemu docker image
.PHONY: distclean-docker-qemu
distclean-docker-qemu: clean-docker-qemu
	-docker image remove "lifesensor/qemu:$(QEMU_COMMIT)"

TARGET_distclean += distclean-docker-vscode
HELP_distclean-docker-vscode = removes vscode docker image
.PHONY: distclean-docker-vscode
distclean-docker-vscode: clean-docker-vscode
	-docker image remove "lifesensor/vscode:latest"

.PHONY: clean-check
clean-check:
	@true

TARGET_distclean += distclean-check
HELP_distclean-check = removes check result if docker is installed
.PHONY: distclean-check
distclean-check: distclean-check-docker

.PHONY: distclean-check-docker
distclean-check-docker: distclean-check-docker-installed distclean-check-docker-group

.PHONY: distclean-check-docker-installed
distclean-check-docker-installed:
	rm -f .check-docker-installed

.PHONY: distclean-check-docker-group
distclean-check-docker-group:
	rm -f .check-docker-group

.PHONY: check-docker
check-docker: check-docker-installed check-docker-group

.PHONY: check-docker-installed
check-docker-installed: | ./.check-docker-installed

./.check-docker-installed:
	@if which docker > /dev/null 2>&1; \
	then \
		touch $@; \
	else \
		( \
		echo "########################################"; \
		echo "# DOCKER DOES NOT SEEM TO BE INSTALLED #"; \
		echo "########################################"; \
		echo "the docker binary could not be found in your PATH!"; \
		echo "This usally means docker is simply not installed."; \
		echo -n "I detected your platform as "; \
		if cat /etc/*-release | grep -qi debian; \
		then \
			echo DEBIAN, so please run; \
			echo; \
			make -sn install-docker-debian; \
			echo; \
			echo OR simply; \
			echo; \
			echo make install-docker-debian; \
			echo; \
		elif cat /etc/*-release | grep -qi ubuntu ; \
		then \
			echo UBUNTU, so please run; \
			echo; \
			make -sn install-docker-ubuntu; \
			echo; \
			echo OR simply; \
			echo; \
			echo make install-docker-ubuntu; \
			echo; \
		elif cat /etc/*-release | grep -qi "arch linux" ; \
		then \
			echo ARCHLINUX, so please run; \
			echo; \
			make -sn install-docker-archlinux; \
			echo; \
			echo OR simply; \
			echo; \
			echo make install-docker-archlinux; \
			echo; \
		elif cat /etc/*-release | grep -qi centos ; \
		then \
			echo CENTOS, so please run; \
			echo; \
			make -sn install-docker-centos; \
			echo; \
			echo OR simply; \
			echo; \
			echo make install-docker-centos; \
			echo; \
		elif cat /etc/*-release | grep -qi fedora ; \
		then \
			echo FEDORA, so please run; \
			echo; \
			make -sn install-docker-fedora; \
			echo; \
			echo OR simply; \
			echo; \
			echo make install-docker-fedora; \
			echo; \
		else \
			echo UNKNOWN; \
			echo please run your package manager to install docker; \
		fi; \
		) 1>&2; \
		exit 1; \
	fi

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

.PHONY: check-docker-group
check-docker-group: | ./.check-docker-group

./.check-docker-group:
	@if groups | grep -q docker; \
	then \
		touch $@; \
	else \
		( \
		echo "##############################################"; \
		echo "# YOUR USER IS NOT PART OF THE DOCKER GROUP! #"; \
		echo "##############################################"; \
		echo "This means you need to execute every docker command with sudo"; \
		echo "To avoid this, please add your user ($$USER) to the docker group with the following command:"; \
		echo; \
		echo "sudo usermod -a -G docker $$USER"; \
		echo; \
		) 1>&2; \
		exit 1; \
	fi

