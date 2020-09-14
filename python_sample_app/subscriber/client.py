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
"""EIS Message Bus echo service client Python example.
"""

import time
import eis.msgbus as mb
import os
import json
from distutils.util import strtobool
from eis.env_config import EnvConfig
from eis.config_manager import ConfigManager
import cfgmgr.config_manager as cfg
from util.util import Util


def start_client():
    msgbus = None
    service = None

    try:
        print('[INFO] Initializing message bus context')

        ctx = cfg.ConfigMgr()
        client_ctx = ctx.get_client_by_name("Visualizer")
        msgbus_cfg = client_ctx.get_msgbus_config()

        msgbus = mb.MsgbusContext(msgbus_cfg)
        print(f'[INFO] Initializing service for {"echo_service"}')

        service = msgbus.get_service("echo_service")
        # Request used for the example
        request = {'int': 42, 'float': 55.5,
                   'str': 'Hello, World!', 'bool': True}

        app_cfg = ctx.get_app_config()
        print(f'App Config is  \'{app_cfg}\'')

        print('[INFO] Running...')
        while True:
            print(f'[INFO] Sending request {request}')
            service.request(request)
            print('[INFO] Waiting for response')
            response, _ = service.recv()
            print(f'[INFO] Received response: {response}')
            time.sleep(int(app_cfg["loop_interval"]))
    except KeyboardInterrupt:
        print('[INFO] Quitting...')
    finally:
        if service is not None:
            service.close()
