.PHONY: default
default: docker-platformio docker-qemu

.PHONY: help
help:
	@echo "--- standard targets ---"
	@echo "make help              | shows this help"
	@echo "make default           | builds platformio,qemu container"
	@echo "make all               | builds all docker containers (platformio,qemu,vscode)"
	@echo "make setup             | checks the setup"
	@echo "make clean             | removes last_vscode_run image"
	@echo "make distclean         | removes all generated docker images"
	@echo
	@echo "--- docker build targets ---"
	@echo "make docker-platformio | builds platformio container"
	@echo "make docker-qemu       | builds qemu container"
	@echo "make docker-vscode     | builds vscode container"
	@echo
	@echo "--- docker run targets ---"
	@echo "make shell-platformio  | starts bash in one-time platformio container"
	@echo "make shell-qemu        | starts bash in one-time qemu container"
	@echo "make shell-vscode      | starts bash in one-time vscode container"
	@echo "make vscode            | starts vscode and saves state in last_vscode_run image"
	@echo
	@echo "--- setup targets ---"
	@echo "make check-docker      | checks docker setup"

.PHONY: all
all: docker-platformio docker-qemu docker-vscode

.PHONY: setup
setup: check-docker

.PHONY: docker-platformio
docker-platformio: platformio/Dockerfile check-docker
	docker build -t esp32:platformio platformio

.PHONY: docker-qemu
docker-qemu: qemu/Dockerfile check-docker
	docker build -t esp32:qemu qemu

.PHONY: docker-vscode
docker-vscode: vscode/Dockerfile docker-qemu check-docker
	docker build -t esp32:vscode vscode

.PHONY: shell-platformio
shell-platformio: docker-platformio check-docker
	docker run --rm -ti esp32:platformio bash

.PHONY: shell-qemu
shell-qemu: docker-qemu check-docker
	docker run --rm -ti esp32:qemu bash

.PHONY: shell-vscode
shell-vscode: docker-vscode check-docker
	xhost local:root
	docker run -ti --rm\
		-v /tmp/.X11-unix/:/tmp/.X11-unix \
		-e DISPLAY=$(DISPLAY) \
		-e LD_PRELOAD=/usr/lib/x86_64-linux-gnu/libglib-2.0.so.0 \
		-v $(shell pwd)/..:/mnt:rw \
		-w /mnt \
		--shm-size=512m \
		esp32:vscode \
		bash

.PHONY: vscode
vscode: docker-vscode check-docker
	xhost local:root
	docker image inspect esp32:last_vscode_run > /dev/null 2>&1 || docker build -t esp32:last_vscode_run vscode
	-docker container rm last_vscode_run
	docker run -ti \
		-v /tmp/.X11-unix/:/tmp/.X11-unix \
		-e DISPLAY=$(DISPLAY) \
		-e LD_PRELOAD=/usr/lib/x86_64-linux-gnu/libglib-2.0.so.0 \
		-e START_VSCODE=1\
		-v $(shell pwd)/..:/mnt:rw \
		-w /mnt \
		--shm-size=512m \
		--name last_vscode_run \
		esp32:last_vscode_run \
		bash
	docker commit last_vscode_run esp32:last_vscode_run
	docker container rm last_vscode_run

.PHONY: clean
clean: 
	-docker image remove esp32:last_vscode_run

.PHONY: distclean
distclean: clean
	-docker image remove esp32:vscode
	-docker image remove esp32:qemu
	-docker image remove esp32:platformio
	docker image prune

.PHONY: check-docker
check-docker: check-docker-installed check-docker-group

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

.PHONY: check-docker-installed
check-docker-installed:
	@echo -n "checking if docker is installed: "
	@if which docker > /dev/null 2>&1; \
	then \
		echo "yes"; \
	else \
		echo "NO!"; \
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

.PHONY: check-docker-group
check-docker-group:
	@echo -n "checking if user is in docker group: "
	@if groups | grep -q docker; \
	then \
		echo "yes"; \
	else \
		echo "NO!"; \
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


