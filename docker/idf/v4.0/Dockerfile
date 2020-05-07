FROM lifesensor/base/v1:latest

RUN sudo -HE apt-get -y install \
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
wget \
;

RUN sudo -HE update-alternatives --install /usr/bin/python python /usr/bin/python3 10

ARG IDF_VERSION
RUN sudo git clone --depth 1 -b ${IDF_VERSION} \
--recursive https://github.com/espressif/esp-idf.git /opt/esp-idf
COPY idf_tools.py.patch /opt/esp-idf/idf_tools.py.patch
ENV IDF_TOOLS_PATH=/opt/espressif IDF_PATH=/opt/esp-idf/
RUN sudo git -C ${IDF_PATH} apply idf_tools.py.patch \
 && sudo -HE python ${IDF_PATH}/tools/idf_tools.py --non-interactive install \
 && sudo rm -rf ${IDF_TOOLS_PATH}/dist \
 && sudo -HE python ${IDF_PATH}/tools/idf_tools.py --non-interactive install-python-env \
 && python ${IDF_PATH}/tools/idf_tools.py export >> ~/.bashrc \
;
RUN echo '[ -n "$IDF_PORT" ] && sudo chgrp developer $IDF_PORT' >> ~/.bashrc
RUN mkdir ~/.ccache