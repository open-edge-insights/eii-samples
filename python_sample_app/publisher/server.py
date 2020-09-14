# Copyright (c) 2020 Intel Corporation.
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
"""EIS Message Bus echo service Python example.
"""

import eis.msgbus as mb
import os
from distutils.util import strtobool
from eis.env_config import EnvConfig
from eis.config_manager import ConfigManager
import cfgmgr.config_manager as cfg
from util.util import Util


def start_server():

    msgbus = None
    service = None

    try:
        print('[INFO] Initializing message bus context')

        ctx = cfg.ConfigMgr()
        server_ctx = ctx.get_server_by_name("echo_service")
        msgbus_cfg = server_ctx.get_msgbus_config()
        msgbus = mb.MsgbusContext(msgbus_cfg)

        print(f'[INFO] Initializing service for PythonServer')
        service = msgbus.new_service("echo_service")

        print('[INFO] Running...')
        while True:
            request, _ = service.recv()
            print(f'[INFO] Received request from client: {request}')
            service.response(request)
    except KeyboardInterrupt:
        print('[INFO] Quitting...')
    finally:
        if service is not None:
            service.close()
