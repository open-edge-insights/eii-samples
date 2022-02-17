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


ARG FEDORA_IMAGE_VERSION
FROM fedora:${FEDORA_IMAGE_VERSION} as builder

LABEL description="GO fedora sample publisher image"

ARG CMAKE_INSTALL_PREFIX
ENV CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}

# Install all compiling tools
RUN dnf groupinstall -y "Development Tools" "Development Libraries"

# Install a few other build-only requirements
RUN dnf install -y cmake gcc-c++

# Install common dependencies across build and runtime
RUN dnf install -y cjson-devel zeromq zlib-devel wget pkg-config

WORKDIR /app

ARG PKG_SRC
RUN wget ${PKG_SRC}/rpms/eii-utils-3.0.0-Linux.rpm && \
    wget ${PKG_SRC}/rpms/eii-messagebus-3.0.0-Linux.rpm && \
    wget ${PKG_SRC}/rpms/eii-configmanager-3.0.0-Linux.rpm

RUN rpm -i /app/eii-utils-3.0.0-Linux.rpm && \
    rpm --force -i /app/eii-messagebus-3.0.0-Linux.rpm && \
    rpm --force -i /app/eii-configmanager-3.0.0-Linux.rpm

RUN ldd /usr/lib/libeiimsgbus.so

RUN wget ${PKG_SRC}/gomod/eii-messagebus-go.tar.gz && \
    wget ${PKG_SRC}/gomod/eii-configmgr-go.tar.gz

RUN tar xvzf eii-messagebus-go.tar.gz && \
    tar xvzf eii-configmgr-go.tar.gz

RUN dnf -y update && \
    dnf -y install go

ENV CGO_CFLAGS="$CGO_FLAGS -O2 -D_FORTIFY_SOURCE=2 -fstack-protector-strong -fPIC" \
    CGO_LDFLAGS="$CGO_LDFLAGS -z noexecstack -z relro -z now"

ENV PATH="$PATH:/usr/local/go/bin" \
    PKG_CONFIG_PATH="$PKG_CONFIG_PATH:${CMAKE_INSTALL_PREFIX}/lib/pkgconfig:/usr/lib/pkgconfig" \
    LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:${CMAKE_INSTALL_PREFIX}/lib"

# These flags are needed for enabling security while compiling and linking with cpuidcheck in golang
ENV CGO_CFLAGS="$CGO_FLAGS -I ${CMAKE_INSTALL_PREFIX}/include -O2 -D_FORTIFY_SOURCE=2 -fstack-protector-strong -fPIC" \
    CGO_LDFLAGS="$CGO_LDFLAGS -L${CMAKE_INSTALL_PREFIX}/lib -z noexecstack -z relro -z now"

COPY ./publisher/go/ ./

RUN go mod tidy

RUN GO111MODULE=on go build -o /app/main ./*.go

FROM fedora:${FEDORA_IMAGE_VERSION} as runtime

# Install dependencies
RUN dnf install -y cjson zeromq zlib

WORKDIR /app

# Create EII user
ARG EII_UID
ARG EII_USER_NAME
RUN groupadd $EII_USER_NAME -g $EII_UID
RUN useradd -r -u $EII_UID -g $EII_USER_NAME $EII_USER_NAME

COPY --from=builder /app/*.rpm /app/

RUN rpm -i /app/eii-utils-3.0.0-Linux.rpm && \
    rpm --force -i /app/eii-messagebus-3.0.0-Linux.rpm && \
    rpm --force -i /app/eii-configmanager-3.0.0-Linux.rpm

COPY --from=builder /app/* /app/
USER $EII_USER_NAME

ENV LD_LIBRARY_PATH ${LD_LIBRARY_PATH}:/app/.local/lib
HEALTHCHECK NONE
ENTRYPOINT ["./main"]