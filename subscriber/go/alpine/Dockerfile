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


ARG GO_VERSION
FROM golang:${GO_VERSION}-alpine as builder

LABEL description="GO alpine sample subscriber image"

ARG CMAKE_INSTALL_PREFIX
ENV CMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}

ENV PATH="$PATH:/usr/local/go/bin" \
    PKG_CONFIG_PATH="$PKG_CONFIG_PATH:${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" \
    LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:${CMAKE_INSTALL_PREFIX}/lib"

# # These flags are needed for enabling security while compiling and linking with cpuidcheck in golang
ENV CGO_CFLAGS="$CGO_FLAGS -I ${CMAKE_INSTALL_PREFIX}/include -O2 -D_FORTIFY_SOURCE=2 -Werror=format-security -fstack-protector-strong -fPIC" \
    CGO_LDFLAGS="$CGO_LDFLAGS -L${CMAKE_INSTALL_PREFIX}/lib -z noexecstack -z relro -z now"

WORKDIR /app

RUN apk add cmake make gcc g++ cjson-dev wget

ARG PKG_SRC
RUN wget ${PKG_SRC}/apks/eii-utils-2.6.0-r0.apk && \
    wget ${PKG_SRC}/apks/eii-messagebus-2.6.0-r0.apk && \
    wget ${PKG_SRC}/apks/eii-configmanager-2.6.0-r0.apk

RUN apk add --allow-untrusted /app/eii-utils-2.6.0-r0.apk && \
    apk add --allow-untrusted /app/eii-messagebus-2.6.0-r0.apk && \
    apk add --allow-untrusted /app/eii-configmanager-2.6.0-r0.apk

RUN wget ${PKG_SRC}/gomod/eii-messagebus-go.tar.gz && \
    wget ${PKG_SRC}/gomod/eii-configmgr-go.tar.gz

RUN tar xvzf eii-messagebus-go.tar.gz && \
    tar xvzf eii-configmgr-go.tar.gz

COPY ./subscriber/go/ ./

RUN go get -u && \
    go mod tidy

RUN GO111MODULE=on go build -o /app/main ./*.go

FROM golang:${GO_VERSION}-alpine as runtime

ARG EII_UID
ARG EII_USER_NAME
RUN adduser -u ${EII_UID} -D -h /home/${EII_USER_NAME} ${EII_USER_NAME}

WORKDIR /app

COPY --from=builder /app/*.apk /app/

RUN apk add --allow-untrusted /app/eii-utils-2.6.0-r0.apk && \
    apk add --allow-untrusted /app/eii-messagebus-2.6.0-r0.apk && \
    apk add --allow-untrusted /app/eii-configmanager-2.6.0-r0.apk

USER $EII_USER_NAME

COPY --from=builder /app/* /app/

ENV LD_LIBRARY_PATH ${LD_LIBRARY_PATH}:/app/.local/lib
HEALTHCHECK NONE
ENTRYPOINT ["./main"]

