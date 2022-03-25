# Copyright (c) 2021 Intel Corporation.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

ARG UBUNTU_IMAGE_VERSION
FROM ubuntu:${UBUNTU_IMAGE_VERSION} as builder

ENV DEBIAN_FRONTEND=noninteractive

RUN apt update && apt install -y build-essential \
    cmake \
    wget

WORKDIR /app

RUN rm -rf deps && \
    mkdir -p deps && \
    cd deps && \
    wget -q --show-progress https://github.com/zeromq/libzmq/releases/download/v4.3.4/zeromq-4.3.4.tar.gz -O zeromq.tar.gz && \
    tar xf zeromq.tar.gz && \
    cd zeromq-4.3.4 && \
    ./configure --prefix=/usr && \
    make install

# Install cjson
RUN rm -rf deps && \
    mkdir -p deps && \
    cd deps && \
    wget -q --show-progress https://github.com/DaveGamble/cJSON/archive/v1.7.12.tar.gz -O cjson.tar.gz && \
    tar xf cjson.tar.gz && \
    cd cJSON-1.7.12 && \
    mkdir build && cd build && \
    cmake -DCMAKE_INSTALL_INCLUDEDIR=/usr/include -DCMAKE_INSTALL_PREFIX=/usr .. && \
    make install

ARG PKG_SRC
RUN wget ${PKG_SRC}/eii-utils-3.0.0-Linux.deb && \
    wget ${PKG_SRC}/eii-messagebus-3.0.0-Linux.deb && \
    wget ${PKG_SRC}/eii-configmanager-3.0.0-Linux.deb

RUN dpkg -i /app/eii-utils-3.0.0-Linux.deb && \
    dpkg -i /app/eii-messagebus-3.0.0-Linux.deb && \
    dpkg -i /app/eii-configmanager-3.0.0-Linux.deb

COPY ./publisher/cpp/ /app/
RUN mkdir release/

ARG CMAKE_BUILD_TYPE=Release
RUN cd release/ && \
    cmake -DSTATIC_CONFIGMGR=ON -DCMAKE_BUILD_TYPE="${CMAKE_BUILD_TYPE}" .. \
    && make

FROM ubuntu:${UBUNTU_IMAGE_VERSION} as runtime

ENV DEBIAN_FRONTEND=noninteractive

WORKDIR /app

COPY --from=builder /app/*.deb /app/
COPY --from=builder /usr/lib /usr/lib
COPY --from=builder /usr/include /usr/include

RUN dpkg -i /app/eii-utils-3.0.0-Linux.deb && \
    dpkg -i /app/eii-messagebus-3.0.0-Linux.deb && \
    dpkg -i /app/eii-configmanager-3.0.0-Linux.deb

ARG EII_UID
ARG EII_USER_NAME
RUN groupadd $EII_USER_NAME -g $EII_UID
RUN useradd -r -u $EII_UID -g $EII_USER_NAME $EII_USER_NAME

USER $EII_USER_NAME

COPY --from=builder /app/release/run /app/run

HEALTHCHECK NONE
ENTRYPOINT ["/app/run"]