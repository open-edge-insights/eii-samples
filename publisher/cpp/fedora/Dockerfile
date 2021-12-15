# Copyright (c) 2021 Intel Corporation.

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

# Dockerfile for InfluxDBConnector

ARG FEDORA_IMAGE_VERSION
FROM fedora:${FEDORA_IMAGE_VERSION} as builder

# Install all compiling tools
RUN dnf groupinstall -y "Development Tools" "Development Libraries"

# Install a few other build-only requirements
RUN dnf install -y cmake gcc-c++

# Install common dependencies across build and runtime
RUN dnf install -y cjson-devel zeromq zlib-devel wget

WORKDIR /app

ARG PKG_SRC
RUN wget ${PKG_SRC}/rpms/eii-utils-2.6.0-Linux.rpm && \
    wget ${PKG_SRC}/rpms/eii-messagebus-2.6.0-Linux.rpm && \
    wget ${PKG_SRC}/rpms/eii-configmanager-2.6.0-Linux.rpm

RUN rpm -i /app/eii-utils-2.6.0-Linux.rpm && \
    rpm --force -i /app/eii-messagebus-2.6.0-Linux.rpm && \
    rpm --force -i /app/eii-configmanager-2.6.0-Linux.rpm

RUN ldd /usr/lib/libeiimsgbus.so

# Build the application
ARG CMAKE_BUILD_TYPE=Release

COPY ./publisher/cpp/ /app/

RUN mkdir release/ && \
    cd release/ && \
    cmake .. -DCMAKE_BUILD_TYPE="${CMAKE_BUILD_TYPE}" && \
    make

FROM fedora:${FEDORA_IMAGE_VERSION} as runtime

# Install dependencies
RUN dnf install -y cjson zeromq zlib

# Create EII user
ARG EII_UID
ARG EII_USER_NAME
RUN groupadd $EII_USER_NAME -g $EII_UID
RUN useradd -r -u $EII_UID -g $EII_USER_NAME $EII_USER_NAME

COPY --from=builder /app/*.rpm /app/

RUN rpm -i /app/eii-utils-2.6.0-Linux.rpm && \
    rpm --force -i /app/eii-messagebus-2.6.0-Linux.rpm && \
    rpm --force -i /app/eii-configmanager-2.6.0-Linux.rpm

USER $EII_USER_NAME

COPY --from=builder /app/release/run /app/run

ENTRYPOINT ["/app/run"]