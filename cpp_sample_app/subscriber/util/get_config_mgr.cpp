// Copyright (c) 2020 Intel Corporation.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM,OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.

#include <eis/config_manager/env_config.h>
#include "eis/msgbus/msgbus.h"

static config_mgr_t* g_config_mgr = NULL;

config_mgr_t* get_config_mgr() {
    std::string pub_cert_file = "";
    std::string pri_key_file = "";
    std::string trust_file = "";
    std::string m_app_name = getenv("AppName");
    bool dev_mode;

    std::string dev_mode_str = getenv("DEV_MODE");
    if (dev_mode_str == "false") {
        dev_mode = false;
    } else if (dev_mode_str == "true") {
        dev_mode = true;
    }

    if (!dev_mode) {
        pub_cert_file = "/run/secrets/etcd_" + m_app_name + "_cert";
        pri_key_file = "/run/secrets/etcd_" + m_app_name + "_key";
        trust_file = "/run/secrets/ca_etcd";
    }

    g_config_mgr = config_mgr_new("etcd",
                                 (char*)(pub_cert_file.c_str()),
                                 (char*)(pri_key_file.c_str()),
                                 (char*)(trust_file.c_str()));
    return g_config_mgr;
}
