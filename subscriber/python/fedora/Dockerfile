# Copyright (c) 2022 Intel Corporation.
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
FROM fedora:${FEDORA_IMAGE_VERSION} as runtime
WORKDIR /app
COPY ./subscriber/python/* /app/

# Install dependencies
RUN dnf install -y cjson zeromq zlib wget python3.9-pip && \
    python3.9 -m pip list

ARG PKG_SRC
RUN wget ${PKG_SRC}/rpms/eii-utils-3.0.0-Linux.rpm && \
    wget ${PKG_SRC}/rpms/eii-messagebus-3.0.0-Linux.rpm && \
    wget ${PKG_SRC}/rpms/eii-configmanager-3.0.0-Linux.rpm

# Installation of eiimessagebus and configmgr whl packages
RUN rpm -i /app/eii-utils-3.0.0-Linux.rpm && \
    rpm --force -i /app/eii-messagebus-3.0.0-Linux.rpm && \
    rpm --force -i /app/eii-configmanager-3.0.0-Linux.rpm

ARG PYPI_SRC
ARG MSGBUS_WHL
ARG CFGMGR_WHL
RUN if [ -z $PYPI_SRC ] ; then \
        pip3 install eii-msgbus && \
        pip3 install eii-configmgr; \
    else \
        wget ${PYPI_SRC}/${MSGBUS_WHL} && \
        wget ${PYPI_SRC}/${CFGMGR_WHL} && \
        pip3 install ${MSGBUS_WHL} && \
        pip3 install ${CFGMGR_WHL} ; \
    fi

# Create EII user
ARG EII_UID
ARG EII_USER_NAME
RUN groupadd $EII_USER_NAME -g $EII_UID
RUN useradd -r -u $EII_UID -g $EII_USER_NAME $EII_USER_NAME

USER $EII_USER_NAME

HEALTHCHECK NONE
ENTRYPOINT ["python3.9", "-u", "main.py"]