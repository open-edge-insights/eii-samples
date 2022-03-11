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

COPY ./publisher/python/*.py /app/

FROM ubuntu:${UBUNTU_IMAGE_VERSION} as runtime

ENV DEBIAN_FRONTEND=noninteractive

RUN apt update && \
    apt install -y  wget

ARG PYTHON_VERSION
RUN apt-get install -y --no-install-recommends python${PYTHON_VERSION} \
                       python${PYTHON_VERSION}-dev \
                       python3-distutils \
                       python3-setuptools \
                       python3-pip

WORKDIR /app

ARG PKG_SRC
RUN wget ${PKG_SRC}/debs/eii-utils-3.0.0-Linux.deb && \
    wget ${PKG_SRC}/debs/eii-messagebus-3.0.0-Linux.deb && \
    wget ${PKG_SRC}/debs/eii-configmanager-3.0.0-Linux.deb

# Installation of utils, eiimessagebus and configmgr debian packages
RUN dpkg -i /app/eii-utils-3.0.0-Linux.deb && \
    dpkg -i /app/eii-messagebus-3.0.0-Linux.deb && \
    dpkg -i /app/eii-configmanager-3.0.0-Linux.deb

ENV PYTHONPATH $PYTHONPATH:/usr/local/lib/python3.8/dist-packages:/app/.local/lib/python3.8/site-packages:/app

ARG PYPI_SRC
ARG MSGBUS_WHL
ARG CFGMGR_WHL

# Installation of eiimessagebus and configmgr whl packages
COPY ./publisher/python/requirements.txt /app/
RUN if [ -z $PYPI_SRC ] ; then \
        pip3 install -r requirements.txt; \
    else \
        wget ${PYPI_SRC}/${MSGBUS_WHL} && \
        wget ${PYPI_SRC}/${CFGMGR_WHL} && \
        pip3 install ${MSGBUS_WHL} && \
        pip3 install ${CFGMGR_WHL} ; \
    fi

RUN apt-get remove --auto-remove --purge -y python3-pip

ARG EII_UID
ARG EII_USER_NAME
RUN groupadd $EII_USER_NAME -g $EII_UID && \
    useradd -r -u $EII_UID -g $EII_USER_NAME $EII_USER_NAME
USER $EII_USER_NAME

COPY --from=builder /app/ /app/
COPY --from=builder /usr/lib /usr/lib
COPY --from=builder /usr/include /usr/include

HEALTHCHECK NONE
ENTRYPOINT ["python3", "-u", "main.py"]