FROM ubuntu:18.04
RUN apt-get update && apt-get -y install \
bison \
ccache \
cmake \
flex \
git \
gperf \
libffi-dev \
libncurses5-dev \
libssl-dev \
libusb-1.0-0 \
ninja-build \
python \
python-pip \
python-setuptools \
python3 \
python3-pip \
python3-setuptools \
sudo \
wget

RUN update-alternatives --install /usr/bin/python python /usr/bin/python3 10

ARG UID=1000
ARG GID=1000
RUN groupadd -g $GID -o developer
RUN useradd -m -u $UID -g $GID -s /bin/bash developer
RUN echo "developer ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers

WORKDIR /home/developer
USER developer
ENV LC_ALL=C.UTF-8 LANG=C.UTF-8

ARG idf_version
RUN sudo git clone --depth 1 -b ${idf_version} \
--recursive https://github.com/espressif/esp-idf.git /opt/esp-idf
WORKDIR /opt/esp-idf
COPY idf_tools.py.patch idf_tools.py.patch
RUN sudo git apply idf_tools.py.patch
ENV IDF_TOOLS_PATH=/opt/espressif
RUN sudo -HE python tools/idf_tools.py --non-interactive install \
&& sudo rm -rf ${IDF_TOOLS_PATH}/dist
RUN sudo -HE python tools/idf_tools.py --non-interactive install-python-env
RUN python tools/idf_tools.py export >> ~/.bashrc

WORKDIR /home/developer
