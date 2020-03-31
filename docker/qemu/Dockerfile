ARG idf_version
FROM lifesensor/idf:${idf_version}

RUN sudo apt-get -y install \
bison \
curl \
flex \
gettext \
libcairo2-dev \
libffi-dev \
libglib2.0-dev \
libgsl-dev \
libreadline-dev \
libxml2-dev \
pkg-config \
zlib1g-dev

RUN curl -L \
http://ftp.gnome.org/pub/gnome/sources/glib/2.48/glib-2.48.0.tar.xz \
| unxz | sudo tar -xC /opt
WORKDIR /opt/glib-2.48.0/
RUN sudo ./configure
RUN sudo make -j4
RUN sudo make install

ARG qemu_version
RUN sudo git clone --depth 1 -b ${qemu_version} \
--recursive https://github.com/espressif/qemu.git /opt/qemu
WORKDIR /opt/qemu
RUN sudo mkdir build
WORKDIR  build
RUN sudo ../configure \
--target-list=xtensa-softmmu \
--enable-debug \
--enable-sanitizers \
--disable-strip \
--disable-user \
--disable-capstone \
--disable-vnc \
--disable-sdl \
--disable-gtk
RUN sudo make -j4
RUN sudo make install

WORKDIR /home/developer
