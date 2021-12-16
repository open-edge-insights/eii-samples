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
    libcjson-dev \
    libzmq5 \
    zlib1g \
    wget \
    pkg-config

WORKDIR /app

ARG PKG_SRC
RUN wget ${PKG_SRC}/debs/eii-utils-2.6.0-Linux.deb && \
    wget ${PKG_SRC}/debs/eii-messagebus-2.6.0-Linux.deb && \
    wget ${PKG_SRC}/debs/eii-configmanager-2.6.0-Linux.deb

RUN dpkg -i /app/eii-utils-2.6.0-Linux.deb && \
    dpkg -i /app/eii-messagebus-2.6.0-Linux.deb && \
    dpkg -i /app/eii-configmanager-2.6.0-Linux.deb

RUN wget ${PKG_SRC}/gomod/eii-messagebus-go.tar.gz && \
    wget ${PKG_SRC}/gomod/eii-configmgr-go.tar.gz

RUN tar xvzf eii-messagebus-go.tar.gz && \
    tar xvzf eii-configmgr-go.tar.gz

ARG GO_VERSION
RUN wget -q --show-progress https://dl.google.com/go/go${GO_VERSION}.linux-amd64.tar.gz && \
    tar -C /usr/local -xzf go${GO_VERSION}.linux-amd64.tar.gz && \
    rm -rf go${GO_VERSION}.linux-amd64.tar.gz

ENV PATH="$PATH:/usr/local/go/bin" \
    PKG_CONFIG_PATH="$PKG_CONFIG_PATH:${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" \
    LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:${CMAKE_INSTALL_PREFIX}/lib"

# These flags are needed for enabling security while compiling and linking with cpuidcheck in golang
ENV CGO_CFLAGS="$CGO_FLAGS -I ${CMAKE_INSTALL_PREFIX}/include -O2 -D_FORTIFY_SOURCE=2 -Werror=format-security -fstack-protector-strong -fPIC" \
    CGO_LDFLAGS="$CGO_LDFLAGS -L${CMAKE_INSTALL_PREFIX}/lib -z noexecstack -z relro -z now"

COPY ./subscriber/go/ ./

RUN GO111MODULE=on go build -o /app/main ./*.go

FROM ubuntu:${UBUNTU_IMAGE_VERSION} as runtime

ENV DEBIAN_FRONTEND=noninteractive
RUN apt update && \
    apt install -y libcjson1 \
    libzmq5 \
    zlib1g

WORKDIR /app
COPY --from=builder /app/*.deb /app/

RUN dpkg -i /app/eii-utils-2.6.0-Linux.deb && \
    dpkg -i /app/eii-messagebus-2.6.0-Linux.deb && \
    dpkg -i /app/eii-configmanager-2.6.0-Linux.deb
    
ARG EII_UID
ARG EII_USER_NAME
RUN groupadd $EII_USER_NAME -g $EII_UID && \
    useradd -r -u $EII_UID -g $EII_USER_NAME $EII_USER_NAME
RUN apt update && apt install --no-install-recommends -y libcjson1 libzmq5 zlib1g && \
    rm -rf /var/lib/apt/lists/*

COPY --from=builder /app/* /app/
USER $EII_USER_NAME

ENV LD_LIBRARY_PATH ${LD_LIBRARY_PATH}:/app/.local/lib
HEALTHCHECK NONE
ENTRYPOINT ["./main"]