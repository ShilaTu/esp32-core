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

.PHONY: docker
TARGET += docker
HELP_docker = build all docker containers
docker: platformio-image qemu-image vscode-image

.PHONY: platformio-image
ALL += platformio-image
DEFAULT += platformio-image
TARGET_docker += platformio-image
HELP_platformio-image = builds docker platformio image
platformio-image: | check-docker
	docker build \
		--build-arg=platformio_version=$(PLATFORMIO_VERSION) \
		--build-arg=UID=$(UID) \
		--build-arg=GID=$(GID) \
		-t "lifesensor/platformio:$(PLATFORMIO_VERSION)" \
		platformio

.PHONY: qemu-image
ALL += qemu-image
DEFAULT += qemu-image
TARGET_docker += qemu-image
HELP_qemu-image = buils docker qemu image
qemu-image: platformio-image | check-docker
	docker build \
		--build-arg=platformio_version=$(PLATFORMIO_VERSION) \
		--build-arg=qemu_commit=$(QEMU_COMMIT) \
		-t "lifesensor/qemu:$(QEMU_COMMIT)" \
		qemu

.PHONY: vscode-image
ALL += vscode-image
TARGET_docker += vscode-image
HELP_vscode-image = buils docker vscode image
vscode-image:  qemu-image | check-docker
	docker build \
		--build-arg=platformio_version=$(PLATFORMIO_VERSION) \
		--build-arg=qemu_commit=$(QEMU_COMMIT) \
		-t "lifesensor/vscode:latest" \
		vscode
		
.PHONY: pio platformio
TARGET += pio 
HELP_pio = runs platformio container
TARGET += platformio 
HELP_platformio = runs platformio container
pio platformio: | platformio-image check-docker
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

.PHONY: clean
TARGET += clean
HELP_clean = cleans docker volumes
clean: clean-docker

.PHONY: clean-docker
clean-docker: clean-platformio-image clean-qemu-image clean-vscode-image

.PHONY: clean-platformio-image
TARGET_clean += clean-platformio-image
HELP_clean-platformio-image = removes platformio volume
clean-platformio-image:
	-docker volume rm pio-home

.PHONY: clean-qemu-image
TARGET_clean += clean-qemu-image
HELP_clean-qemu-image = removes qemu volume
clean-qemu-image:
	-docker volume rm qemu-home

.PHONY: clean-vscode-image
TARGET_clean += clean-vscode-image
HELP_clean-vscode-image = removes vscode volume
clean-vscode-image:
	-docker volume rm vscode-home

.PHONY: distclean
TARGET += distclean
HELP_distclean = removes all generated files by docker
distclean: clean distclean-check distclean-docker

.PHONY: distclean-docker
distclean-docker: clean-docker distclean-platformio-image distclean-qemu-image distclean-vscode-image
	-docker image prune

.PHONY: distclean-platformio-image
TARGET_distclean += distclean-platformio-image
HELP_distclean-platformio-image = removes platformio docker image
distclean-platformio-image: clean-platformio-image
	-docker image remove "lifesensor/platformio:$(PLATFORMIO_VERSION)"
	
.PHONY: distclean-qemu-image
TARGET_distclean += distclean-qemu-image
HELP_distclean-qemu-image = removes qemu docker image
distclean-qemu-image: clean-qemu-image
	-docker image remove "lifesensor/qemu:$(QEMU_COMMIT)"

.PHONY: distclean-vscode-image
TARGET_distclean += distclean-vscode-image
HELP_distclean-vscode-image = removes vscode docker image
distclean-vscode-image: clean-vscode-image
	-docker image remove "lifesensor/vscode:latest"

.PHONY: clean-check
clean-check:
	@true

.PHONY: distclean-check
TARGET_distclean += distclean-check
HELP_distclean-check = removes check result if docker is installed
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
CHECK += check-docker
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

include ../.Makefile.template
