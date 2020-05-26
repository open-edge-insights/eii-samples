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
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.

// get_app_config gets the app config and returns the sleep value from
// eis_config.json

#include <eis/config_manager/env_config.h>
#include "eis/msgbus/msgbus.h"

int get_app_config(char* cpp_app_config) {
    config_t* config = json_config_new_from_buffer(cpp_app_config);
    if (config == NULL) {
        const char* err = "Failed to initialize configuration object";
        LOG_ERROR("%s", err);
        throw(err);
    }
    config_value_t* loop_interval = config->get_config_value(config->cfg,
                                                            "loop_interval");
    if (loop_interval == NULL) {
        const char* err = "\"loop_interval\" key is missing";
        LOG_ERROR("%s", err);
        throw(err);
    }
    if (loop_interval->type != CVT_STRING) {
        const char* err = "\"type\" value has to be of string type";
        LOG_ERROR("%s", err);
        config_destroy(config);
        config_value_destroy(loop_interval);
        throw(err);
    }
    char* loop_val = loop_interval->body.string;
    int sleep_val = atoi(loop_val);
    return sleep_val;
}
