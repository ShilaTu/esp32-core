FROM ubuntu:18.04
RUN apt-get update && apt-get -y install \
git \
make \
sudo \
;

# creates user 'developer'
ARG UID=1000
ARG GID=1000
RUN groupadd -g $GID -o developer \
 && useradd -m -u $UID -g $GID -s /bin/bash developer \
 && echo "developer ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers
WORKDIR /home/developer
USER developer
ENV LC_ALL=C.UTF-8 LANG=C.UTF-8 DEBIAN_FRONTEND=noninteractive
