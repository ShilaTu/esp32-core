FROM ubuntu:18.04

RUN apt-get update
RUN apt-get -y install python3 python3-pip git sudo

RUN useradd -ms /bin/bash developer
RUN echo "developer ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers

WORKDIR /home/developer
USER developer
ENV LC_ALL=C.UTF-8 LANG=C.UTF-8 
ENV PATH="${PATH}:/home/developer/.local/bin"
ENV PATH="${PATH}:/home/developer/.platformio/packages/toolchain-xtensa32/bin/"

ARG platformio_version
RUN pip3 install --no-cache --upgrade pip setuptools wheel platformio==${platformio_version}
RUN pio platform install espressif32 --with-package framework-espidf
