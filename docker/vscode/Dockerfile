ARG qemu_commit
FROM lifesensor/qemu:${qemu_commit}

RUN sudo apt-get -y install libx11-xcb1 libasound2
RUN \
    curl https://packages.microsoft.com/keys/microsoft.asc | gpg --dearmor | sudo tee /usr/share/keyrings/packages.microsoft.gpg > /dev/null&& \
    sudo chmod 664 /usr/share/keyrings/packages.microsoft.gpg && \
    sudo chown root:root /usr/share/keyrings/packages.microsoft.gpg && \
    echo "deb [arch=amd64 signed-by=/usr/share/keyrings/packages.microsoft.gpg] https://packages.microsoft.com/repos/vscode stable main" | sudo tee /etc/apt/sources.list.d/vscode.list > /dev/null && \
    sudo apt-get -y install apt-transport-https && \
    sudo apt-get update && \
    sudo apt-get -y install code

RUN echo "code /mnt &" >> .bashrc
RUN LD_PRELOAD=/usr/lib/x86_64-linux-gnu/libglib-2.0.so.0 code -w --install-extension platformio.platformio-ide
