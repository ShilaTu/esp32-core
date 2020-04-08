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
| unxz | tar -xC /tmp \
&& cd /tmp/glib-2.48.0/ \
&& ./configure \
&& make -j $(grep -ci processor /proc/cpuinfo) \
&& sudo make install \
&& rm -rf /tmp/glib-2.48.0

ARG qemu_version
RUN git clone --depth 1 -b ${qemu_version} \
--recursive https://github.com/espressif/qemu.git /tmp/qemu \
&& mkdir -p /tmp/qemu/build \
&& cd /tmp/qemu/build \
&& ../configure \
--target-list=xtensa-softmmu \
--enable-debug \
--enable-sanitizers \
--disable-strip \
--disable-user \
--disable-capstone \
--disable-vnc \
--disable-sdl \
--disable-gtk \
&& make -j $(grep -ci processor /proc/cpuinfo) \
&& sudo make install \
&& rm -rf /tmp/qemu

