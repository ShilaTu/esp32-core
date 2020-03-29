FROM ubuntu:18.04

RUN apt-get update &&apt-get -y install ninja-build libncurses5-dev flex bison gperf python python-pip cmake git sudo libusb-1.0-0

ARG UID=1000
ARG GID=1000
RUN groupadd -g $GID -o developer
RUN useradd -m -u $UID -g $GID -s /bin/bash developer
RUN echo "developer ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers

WORKDIR /home/developer
USER developer
ENV LC_ALL=C.UTF-8 LANG=C.UTF-8

ARG idf_version
RUN mkdir ~/git && git clone -b ${idf_version} --recursive https://github.com/espressif/esp-idf.git ~/git/esp-idf
COPY idf_tools.py.patch git/esp-idf/tools/idf_tools.py.patch
RUN cd ~/git/esp-idf && git apply ~/git/esp-idf/tools/idf_tools.py.patch
RUN cd ~/git/esp-idf && ./install.sh
RUN echo "source $HOME/git/esp-idf/export.sh" >> ~/.bashrc
