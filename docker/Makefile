PLATFORMIO_VERSION?=4.1.0
QEMU_COMMIT?=esp-develop
DEV?=/dev/ttyUSB0
EXEC?=bash

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
	@echo "make pio-flash         | starts build and upload proccess"
	@echo "make pio-monitor       | starts minicom serial console"
	@echo "make pio-clean         | remove pio build files"
	@echo
	@echo "--- variables ---"
	@echo "PLATFORMIO_VERSION     | which platformio version to use (currently '$(PLATFORMIO_VERSION)')"
	@echo "QEMU_COMMIT            | which qemu commitish to checkout (currently '$(QEMU_COMMIT)')"
	@echo "DEV                    | which device to use for flashing/monitoring (currently '$(DEV)'"
	@echo "EXEC                   | whhat to execute inside the container (currently '$(EXEC)'"

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

.PHONY: pio-flash pio-upload platformio-flash platformio-upload
pio-flash pio-upload platformio-flash platformio-upload: | docker-platformio check-docker
	docker run --rm -ti \
		--device "${DEV}" \
		--volume "$(shell pwd)/..":/mnt:rw \
		-w /mnt \
		--name flash \
		lifesensor/platformio:$(PLATFORMIO_VERSION) \
		/bin/sh -c \
		"pio run --upload-port '$(DEV)' -t upload"

.PHONY: pio-monitor platformio-monitor
pio-monitor platformio-monitor: | docker-platformio check-docker
	docker run --rm -ti \
		--device "${DEV}" \
		--volume "$(shell pwd)/..":/mnt:rw \
		--name monitor \
		lifesensor/platformio:$(PLATFORMIO_VERSION) \
		/bin/sh -c \
		"python3 \
		/home/developer/.platformio/packages/framework-espidf/tools/idf_monitor.py \
		/mnt/.pio/build/pico32/firmware.elf --port ${DEV}"

.PHONY: pio-clean platformio-clean
pio-clean platformio-clean: | docker-platformio check-docker
	docker run --rm -ti \
		--device "${DEV}" \
		--volume "$(shell pwd)/..":/mnt:rw \
		-w /mnt \
		--name clean \
		lifesensor/platformio:$(PLATFORMIO_VERSION) \
		/bin/sh -c \
		"pio run -t clean"

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
	docker run \
		--rm \
		-ti \
		-v pio-home:/home/developer:rw \
		-v $(shell pwd)/..:/mnt:rw \
		-w /mnt \
		--hostname platformio \
		lifesensor/platformio:$(PLATFORMIO_VERSION) \
		/bin/sh -c 'exec $(EXEC)'

.PHONY: qemu
qemu: | docker-qemu check-docker
	docker run \
	 --rm \
		-ti \
		-v qemu-home:/home/developer:rw \
		-v $(shell pwd)/..:/mnt:rw \
		-w /mnt \
		--hostname qemu \
		lifesensor/qemu:$(QEMU_COMMIT) \
		/bin/sh -c 'exec $(EXEC)'

.PHONY: vscode
code vscode: | docker-vscode check-docker
	xhost local:root
	docker run \
		--rm \
		-ti \
		-v vscode-home:/home/developer:rw \
		-v /tmp/.X11-unix/:/tmp/.X11-unix \
		-e DISPLAY=$(DISPLAY) \
		-e LD_PRELOAD=/usr/lib/x86_64-linux-gnu/libglib-2.0.so.0 \
		-v $(shell pwd)/..:/mnt:rw \
		-w /mnt \
		--shm-size=512m \
		--hostname vscode \
		lifesensor/vscode:latest \
		/bin/sh -c 'exec $(EXEC)'

.PHONY: clean
clean: clean-docker clean-check

.PHONY: clean-docker
clean-docker: clean-docker-platformio clean-docker-qemu clean-docker-vscode

.PHONY: clean-docker-platformio
clean-docker-platformio:
	rm -f platformio/Dockerfile.log
	-docker volume rm pio-home

.PHONY: clean-docker-qemu
clean-docker-qemu:
	rm -f qemu/Dockerfile.log
	-docker volume rm qemu-home

.PHONY: clean-docker-vscode
clean-docker-vscode:
	rm -f vscode/Dockerfile.log
	-docker volume rm vscode-home

.PHONY: distclean
distclean: clean distclean-check distclean-docker

.PHONY: distclean-docker
distclean-docker: clean-docker distclean-docker-platformio distclean-docker-qemu distclean-docker-vscode
	-docker image prune

.PHONY: distclean-docker-platformio
distclean-docker-platformio: clean-docker-platformio
	-docker image remove "lifesensor/platformio:$(PLATFORMIO_VERSION)"
	
.PHONY: distclean-docker-qemu
distclean-docker-qemu: clean-docker-qemu
	-docker image remove "lifesensor/qemu:$(QEMU_COMMIT)"

.PHONY: distclean-docker-vscode
distclean-docker-vscode: clean-docker-vscode
	-docker image remove "lifesensor/vscode:latest"

.PHONY: clean-check
clean-check:
	@true

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

