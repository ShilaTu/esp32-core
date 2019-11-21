FROM esp32:platformio

RUN \
    curl -L http://ftp.gnome.org/pub/gnome/sources/glib/2.48/glib-2.48.0.tar.xz | unxz | sudo tar -xC /opt && \
    cd /opt/glib-2.48.0/ && \
    sudo ./configure && \
    sudo make -j4 && \
    sudo make install

RUN \
    sudo git clone https://github.com/espressif/qemu.git /opt/qemu && \
    cd /opt/qemu && \
    sudo git checkout esp-develop && \
    sudo mkdir build && \
    cd build && \
    sudo ../configure \
        --target-list=xtensa-softmmu \
        --enable-debug \
        --enable-sanitizers \
        --disable-strip \
        --disable-user \
        --disable-capstone \
        --disable-vnc \
        --disable-sdl \
        --disable-gtk && \
    sudo make -j4 && \
    sudo make install
