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
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <atomic>
#include "eii/msgbus/msgbus.h"
#include "eii/utils/logger.h"
#include "eii/utils/json_config.h"
#include "eii/config_manager/config_mgr.hpp"

#ifndef SAMPLES_CPP_SAMPLE_APP_SUBSCRIBER_INCLUDE_CLIENT_H_
#define SAMPLES_CPP_SAMPLE_APP_SUBSCRIBER_INCLUDE_CLIENT_H_

using namespace eii::config_manager;


class Client {
 private:
  ConfigMgr* client_ch = NULL;
  void* g_msgbus_ctx_client = NULL;
  recv_ctx_t* g_service_ctx = NULL;
  msg_envelope_serialized_part_t* parts = NULL;
  msg_envelope_t* msg = NULL;
  int num_parts = 0;
  std::atomic<bool> *loop;

 public:
  explicit Client(std::atomic<bool> *loop);
  ~Client();
  bool init();
  static void* start(void *arg);
  int client_service();
};

#endif  // SAMPLES_CPP_SAMPLE_APP_SUBSCRIBER_INCLUDE_CLIENT_H_

