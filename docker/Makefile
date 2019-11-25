PLATFORMIO_VERSION?=4.1.0
QEMU_COMMIT?=esp-develop

.PHONY: default
default: check docker-platformio docker-qemu

.PHONY: help
help:
	@echo "--- standard targets ---"
	@echo "make help              | shows this help"
	@echo "make default           | builds platformio,qemu container"
	@echo "make all               | builds all docker containers (platformio,qemu,vscode)"
	@echo "make check             | checks the setup"
	@echo "make clean             | removes containers and checks"
	@echo "make distclean         | removes all generated files"
	@echo
	@echo "--- docker build targets ---"
	@echo "make docker-platformio | builds platformio container"
	@echo "make docker-qemu       | builds qemu container"
	@echo "make docker-vscode     | builds vscode container"
	@echo
	@echo "--- docker start targets ---"
	@echo "make platformio        | starts bash in platformio container"
	@echo "make qemu              | starts bash in qemu container"
	@echo "make vscode            | starts bash&vscode in vscode container"
	@echo
	@echo "--- variables ---"
	@echo "PLATFORMIO_VERSION     | which platformio version to use (currently '$(PLATFORMIO_VERSION)')"
	@echo "QEMU_COMMIT            | which qemu commitish to checkout (currently '$(QEMU_COMMIT)')"

.PHONY: all
all: check docker-platformio docker-qemu docker-vscode

.PHONY: check
check: check-docker

.PHONY: docker-platformio
docker-platformio: platformio/Dockerfile.log

platformio/Dockerfile.log: platformio/Dockerfile | check-docker
	docker build \
		--build-arg=platformio_version=$(PLATFORMIO_VERSION) \
		-t "lifesensor/platformio:$(PLATFORMIO_VERSION)" \
		platformio 2>&1 \
	| tee -a $@.part
	mv $@.part $@

.PHONY: docker-qemu
docker-qemu: qemu/Dockerfile.log

qemu/Dockerfile.log: qemu/Dockerfile | docker-platformio check-docker
	docker build \
		--build-arg=platformio_version=$(PLATFORMIO_VERSION) \
		--build-arg=qemu_commit=$(QEMU_COMMIT) \
		-t "lifesensor/qemu:$(QEMU_COMMIT)" \
		qemu \
	| tee -a $@.part
	mv $@.part $@

.PHONY: docker-vscode
docker-vscode: vscode/Dockerfile.log

vscode/Dockerfile.log: vscode/Dockerfile | docker-qemu check-docker
	docker build \
		--build-arg=platformio_version=$(PLATFORMIO_VERSION) \
		--build-arg=qemu_commit=$(QEMU_COMMIT) \
		-t "lifesensor/vscode:latest" \
		vscode \
	| tee -a $@.part
	mv $@.part $@
		

.PHONY: pio platformio
pio platformio: | docker-platformio check-docker
	-docker create \
		-ti \
		-v $(shell pwd)/..:/mnt:rw \
		-w /mnt \
		--name platformio \
		--hostname platformio \
		lifesensor/platformio:$(PLATFORMIO_VERSION) \
		bash
	docker start -ai platformio

.PHONY: qemu
qemu: | docker-qemu check-docker
	-docker create \
		-ti \
		-v $(shell pwd)/..:/mnt:rw \
		-w /mnt \
		--name qemu\
		--hostname qemu \
		lifesensor/qemu:$(QEMU_COMMIT) \
		bash
	docker start -ai qemu

.PHONY: vscode
code vscode: | docker-vscode check-docker
	xhost local:root
	-docker create \
		-ti \
		-v /tmp/.X11-unix/:/tmp/.X11-unix \
		-e DISPLAY=$(DISPLAY) \
		-e LD_PRELOAD=/usr/lib/x86_64-linux-gnu/libglib-2.0.so.0 \
		-v $(shell pwd)/..:/mnt:rw \
		-w /mnt \
		--shm-size=512m \
		--name vscode \
		--hostname vscode \
		lifesensor/vscode:latest \
		bash
	docker start -ai vscode

.PHONY: clean
clean: clean-docker clean-check

.PHONY: clean-docker
clean-docker: clean-docker-platformio clean-docker-qemu clean-docker-vscode

.PHONY: clean-docker-platformio
clean-docker-platformio:
	rm -f platformio/Dockerfile.log
	-docker container rm platformio

.PHONY: clean-docker-qemu
clean-docker-qemu:
	rm -f qemu/Dockerfile.log
	-docker container rm qemu

.PHONY: clean-docker-vscode
clean-docker-vscode:
	rm -f vscode/Dockerfile.log
	-docker container rm vscode

.PHONY: distclean
distclean: clean distclean-docker

.PHONY: clean-docker
distclean-docker: distclean-docker-platformio distclean-docker-qemu distclean-docker-vscode

.PHONY: clean-docker-platformio
distclean-docker-platformio: check-docker
	-docker image remove "lifesensor/platformio:$(PLATFORMIO_VERSION)"
	docker image prune
	
.PHONY: clean-docker-qemu
distclean-docker-qemu: check-docker
	-docker image remove "lifesensor/qemu:latest"
	docker image prune

.PHONY: clean-docker-vscode
distclean-docker-vscode: check-docker
	-docker image remove "lifesensor/vscode:latest"
	docker image prune

.PHONY: clean-check
clean-check: clean-check-docker

.PHONY: clean-check-docker
clean-check-docker: clean-check-docker-installed clean-check-docker-group

.PHONY: clean-check-docker-installed
clean-check-docker-installed:
	rm -f .check-docker-installed

.PHONY: clean-check-docker-group
clean-check-docker-group:
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

