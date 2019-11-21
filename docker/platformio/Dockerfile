FROM ubuntu:18.04

RUN \
    apt-get update && \
    apt-get -y install \
        python3-pip \
        curl \
        zlib1g-dev \
        pkg-config \
        libffi-dev \
        gettext \
        libxml2-dev \
        libcairo2-dev \
        libreadline-dev \
        libglib2.0-dev \
        libgsl-dev \
        git \
        sudo

RUN pip3 install -U PlatformIO

RUN \
    useradd -ms /bin/bash developer && \
    echo "developer ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers

WORKDIR /home/developer
USER developer

RUN \
    echo "export LC_ALL=C.UTF-8" >> .bashrc && \
    echo "export LANG=C.UTF-8" >> .bashrc
