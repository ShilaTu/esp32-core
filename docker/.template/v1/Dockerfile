FROM lifesensor/base:latest

ARG FOO
ARG BAR

###########################
# install needed packages #
###########################
RUN sudo -HE apt-get install -y \
g++ \
cmake \
python3 \
;

###############################################################
# build & install some software                               #
# please build & install & remove in a single RUN instruction #
# to avoid intermediate images                                #
###############################################################
RUN export REPO=/tmp/BAR \
 && git clone https://github.com/FOO/BAR.git $REPO \
 && mkdir $REPO/build \
 && cd $REPO/build \
 && cmake .. \
 && sudo make install \
 && sudo rm -rf $REPO