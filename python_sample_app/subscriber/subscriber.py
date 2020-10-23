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
"""EIS Message Bus subscriber example
"""

import eis.msgbus as mb
import os
from distutils.util import strtobool
import cfgmgr.config_manager as cfg
from util.util import Util


def start_subscriber():
    subscriber = None

    try:
        
        ctx = cfg.ConfigMgr()
        sub_ctx = ctx.get_subscriber_by_name("Cam2_Results")
        msgbus_cfg = sub_ctx.get_msgbus_config()

        print('[INFO] Initializing message bus context')
        msgbus_sub = mb.MsgbusContext(msgbus_cfg)

        topics = sub_ctx.get_topics()

        print(f'[INFO] Initializing subscriber for topic {topics[0]}')
        subscriber = msgbus_sub.new_subscriber(topics[0])

        print('[INFO] Running...')

        while True:
            msg, _ = subscriber.recv()
            if msg is not None:
                print(f'[INFO] RECEIVED by subscriber : {msg}')
            else:
                print('[INFO] Receive interrupted')
    except KeyboardInterrupt:
        print('[INFO] Quitting...')
    finally:
        if subscriber is not None:
            subscriber.close()
