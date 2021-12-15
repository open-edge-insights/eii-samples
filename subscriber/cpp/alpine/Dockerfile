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

ARG ALPINE_IMAGE_VERSION
FROM alpine:${ALPINE_IMAGE_VERSION} as builder

WORKDIR /app

# Build depdencies
RUN apk add cmake make gcc g++ cjson-dev wget

ARG PKG_SRC
RUN wget ${PKG_SRC}/apks/eii-utils-2.6.0-r0.apk && \
    wget ${PKG_SRC}/apks/eii-messagebus-2.6.0-r0.apk && \
    wget ${PKG_SRC}/apks/eii-configmanager-2.6.0-r0.apk

RUN apk add --allow-untrusted /app/eii-utils-2.6.0-r0.apk && \
    apk add --allow-untrusted /app/eii-messagebus-2.6.0-r0.apk && \
    apk add --allow-untrusted /app/eii-configmanager-2.6.0-r0.apk

# Build the application
ARG CMAKE_BUILD_TYPE=Release

COPY ./subscriber/cpp/ /app/
RUN mkdir release/ && \
    cd release/ && \
    cmake .. -DCMAKE_BUILD_TYPE="${CMAKE_BUILD_TYPE}" && \
    make

FROM alpine:${ALPINE_IMAGE_VERSION} as runtime

# Create EII user
ARG EII_UID
ARG EII_USER_NAME
RUN adduser -u ${EII_UID} -D -h /home/${EII_USER_NAME} ${EII_USER_NAME}

COPY --from=builder /app/*.apk /app/
RUN apk add --allow-untrusted /app/eii-utils-2.6.0-r0.apk && \
    apk add --allow-untrusted /app/eii-messagebus-2.6.0-r0.apk && \
    apk add --allow-untrusted /app/eii-configmanager-2.6.0-r0.apk

USER $EII_USER_NAME

COPY --from=builder /app/release/run /app/run

ENTRYPOINT ["/app/run"]