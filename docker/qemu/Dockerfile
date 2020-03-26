ARG idf_version
FROM lifesensor/idf:${idf_version}

RUN sudo apt-get -y install zlib1g-dev pkg-config libffi-dev gettext libxml2-dev libcairo2-dev libreadline-dev libglib2.0-dev libgsl-dev bison flex

RUN curl -L http://ftp.gnome.org/pub/gnome/sources/glib/2.48/glib-2.48.0.tar.xz | unxz | sudo tar -xC /opt
WORKDIR /opt/glib-2.48.0/
RUN sudo ./configure
RUN sudo make -j4
RUN sudo make install

RUN sudo git clone https://github.com/espressif/qemu.git /opt/qemu
WORKDIR /opt/qemu
ARG qemu_commit
RUN sudo git checkout ${qemu_commit}
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
