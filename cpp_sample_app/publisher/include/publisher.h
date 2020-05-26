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

#include <signal.h>
#include <unistd.h>
#include <atomic>
#include <eis/config_manager/env_config.h>
#include "eis/msgbus/msgbus.h"

#ifndef SAMPLES_CPP_SAMPLE_APP_PUBLISHER_INCLUDE_PUBLISHER_H_
#define SAMPLES_CPP_SAMPLE_APP_PUBLISHER_INCLUDE_PUBLISHER_H_

class Publisher {
 private:
  publisher_ctx_t* m_pub_ctx = NULL;
  int num_parts = 0;
  void* g_msgbus_ctx = NULL;
  env_config_t* g_env_config_client = NULL;
  config_mgr_t* g_config_mgr = NULL;
  msg_envelope_serialized_part_t* parts = NULL;
  msg_envelope_t* g_msg = NULL;
  std::atomic<bool> *loop;

 public:
  explicit Publisher(std::atomic<bool> *loop);
  ~Publisher();
  bool init(char *topic_name);
  static void* start(void *arg);
  int publish();
  void clean_up();
};

#endif  // SAMPLES_CPP_SAMPLE_APP_PUBLISHER_INCLUDE_PUBLISHER_H_

