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
"""EII Message Bus echo service Python example.
"""

import eii.msgbus as mb
import os
import cfgmgr.config_manager as cfg


def start_server():

    msgbus = None
    service = None

    try:
        print('[INFO] Initializing message bus context')

        ctx = cfg.ConfigMgr()
        if ctx.get_num_servers() is -1:
            raise "No server instances found, exiting..."
        server_ctx = ctx.get_server_by_index(0)
        msgbus_cfg = server_ctx.get_msgbus_config()
        msgbus = mb.MsgbusContext(msgbus_cfg)

        print(f'[INFO] Initializing service for PythonServer')

        # TODO: dynamically get this value using get_app_interface()
        service = msgbus.new_service("default")

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
