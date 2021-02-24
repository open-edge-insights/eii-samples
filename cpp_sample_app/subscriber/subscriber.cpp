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

/**
 * @brief Message bus Subscriber example
 */

#include <signal.h>
#include <unistd.h>
extern "C" {
    #include <safe_str_lib.h>
}
#include "eii/msgbus/msgbus.h"
#include "subscriber.h"

#define SIZE 100

Subscriber::Subscriber(std::atomic<bool> *loop) {
    sub_ch = new ConfigMgr();
    this->loop = loop;
}

Subscriber::~Subscriber() {
    if (msg != NULL)
        msgbus_msg_envelope_destroy(msg);
    if (parts != NULL)
        msgbus_msg_envelope_serialize_destroy(parts, num_parts);
    if (g_sub_ctx != NULL)
        msgbus_recv_ctx_destroy(g_msgbus_ctx, g_sub_ctx);
    if (g_msgbus_ctx != NULL)
        msgbus_destroy(g_msgbus_ctx);
}

bool Subscriber::init() {
    // SubscriberCfg* sub_ctx = sub_ch->getSubscriberByName("sample_sub");
    SubscriberCfg* sub_ctx = sub_ch->getSubscriberByIndex(0);
    config_t* sub_config = sub_ctx->getMsgBusConfig();

    std::vector<std::string> topics = sub_ctx->getTopics();

    g_msgbus_ctx = msgbus_initialize(sub_config);
    if (g_msgbus_ctx == NULL) {
        LOG_ERROR_0("Failed to initialize message bus");
        goto err;
    }

    msgbus_ret_t ret;
    ret = msgbus_subscriber_new(g_msgbus_ctx, topics[0].c_str(), NULL, &g_sub_ctx);

    if (ret != MSG_SUCCESS) {
        LOG_ERROR("Failed to initialize subscriber (errno: %d)", ret);
        goto err;
    }
    return true;

err:
    if (g_sub_ctx != NULL)
        msgbus_recv_ctx_destroy(g_msgbus_ctx, g_sub_ctx);
    if (g_msgbus_ctx != NULL)
        msgbus_destroy(g_msgbus_ctx);
    return false;
}

void* Subscriber::start(void *arg) {
    int ret_val = -1;
    Subscriber *obj = reinterpret_cast<Subscriber*>(arg);

    if ((ret_val = obj->subscribe()) != 0) {
        LOG_ERROR("subscribe failed.(errno: %d)", ret_val);
    }

    delete obj;
}

int Subscriber::subscribe() {
    int ret = 0;
    LOG_INFO_0("Running...");

    while (loop->load()) {
        ret = msgbus_recv_wait(g_msgbus_ctx, g_sub_ctx, &msg);
        if (ret != MSG_SUCCESS) {
            // Interrupt is an acceptable error
            if (ret == MSG_ERR_EINTR)
                break;
            LOG_ERROR("Failed to receive message (errno: %d)", ret);
            goto err;
        }

        num_parts = msgbus_msg_envelope_serialize(msg, &parts);
        if (num_parts <= 0) {
            LOG_ERROR_0("Failed to serialize message");
            goto err;
        }

        LOG_INFO("Message Received at Subscriber: %s", parts[0].bytes);

        msgbus_msg_envelope_serialize_destroy(parts, num_parts);
        msgbus_msg_envelope_destroy(msg);
        msg = NULL;
        parts = NULL;
    }

    if (parts != NULL)
        msgbus_msg_envelope_serialize_destroy(parts, num_parts);

    return 0;

err:
    if (msg != NULL)
        msgbus_msg_envelope_destroy(msg);
    if (parts != NULL)
        msgbus_msg_envelope_serialize_destroy(parts, num_parts);
    if (g_sub_ctx != NULL)
        msgbus_recv_ctx_destroy(g_msgbus_ctx, g_sub_ctx);
    if (g_msgbus_ctx != NULL)
        msgbus_destroy(g_msgbus_ctx);
    return -1;
}
