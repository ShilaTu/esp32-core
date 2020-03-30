FROM ubuntu:18.04

RUN apt-get update && apt-get -y install ninja-build libncurses5-dev flex bison gperf python3 python3-pip cmake git sudo libusb-1.0-0

RUN ln -sr /usr/bin/python3 /usr/bin/python
RUN ln -sr /usr/bin/pip3 /usr/bin/pip

ARG UID=1000
ARG GID=1000
RUN groupadd -g $GID -o developer
RUN useradd -m -u $UID -g $GID -s /bin/bash developer
RUN echo "developer ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers

WORKDIR /home/developer
USER developer
ENV LC_ALL=C.UTF-8 LANG=C.UTF-8

ARG idf_version
RUN sudo git clone -b ${idf_version} --recursive https://github.com/espressif/esp-idf.git /opt/esp-idf
WORKDIR /opt/esp-idf
COPY idf_tools.py.patch idf_tools.py.patch
RUN sudo git apply idf_tools.py.patch
ENV IDF_TOOLS_PATH=/opt/espressif
RUN sudo -E python3 tools/idf_tools.py install
RUN sudo -E python3 tools/idf_tools.py install-python-env
RUN python3 tools/idf_tools.py export >> ~/.bashrc

WORKDIR /home/developer