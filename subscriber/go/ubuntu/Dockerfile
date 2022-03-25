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

LABEL description="GO Ubuntu sample subscriber image"

ARG CMAKE_INSTALL_PREFIX
ENV CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}

ENV DEBIAN_FRONTEND=noninteractive

RUN apt update && \
    apt install -y build-essential \
    cmake \
    wget \
    pkg-config

WORKDIR /app

ARG CMAKE_INSTALL_PREFIX
# Install libzmq
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

ARG GO_VERSION
RUN wget -q --show-progress https://dl.google.com/go/go${GO_VERSION}.linux-amd64.tar.gz && \
    tar -C /usr/local -xzf go${GO_VERSION}.linux-amd64.tar.gz && \
    rm -rf go${GO_VERSION}.linux-amd64.tar.gz

ENV PATH="$PATH:/usr/local/go/bin" \
    PKG_CONFIG_PATH="$PKG_CONFIG_PATH:${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" \
    LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:${CMAKE_INSTALL_PREFIX}/lib"

# These flags are needed for enabling security while compiling and linking with cpuidcheck in golang
ENV CGO_CFLAGS="$CGO_FLAGS -I ${CMAKE_INSTALL_PREFIX}/include -O2 -D_FORTIFY_SOURCE=2 -Werror=format-security -fstack-protector-strong -fno-strict-overflow -fno-delete-null-pointer-checks -fwrapv -fPIC" \
    CGO_LDFLAGS="$CGO_LDFLAGS -L${CMAKE_INSTALL_PREFIX}/lib -z noexecstack -z relro -z now"

COPY ./subscriber/go/ ./

RUN GO111MODULE=on go build -o /app/main ./*.go

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
RUN groupadd $EII_USER_NAME -g $EII_UID && \
    useradd -r -u $EII_UID -g $EII_USER_NAME $EII_USER_NAME
RUN rm -rf /var/lib/apt/lists/*

COPY --from=builder /app/* /app/
USER $EII_USER_NAME

ENV LD_LIBRARY_PATH ${LD_LIBRARY_PATH}:/app/.local/lib

HEALTHCHECK NONE
ENTRYPOINT ["./main"]