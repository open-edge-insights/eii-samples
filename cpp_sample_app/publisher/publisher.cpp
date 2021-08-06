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
 * @brief Message bus Publisher example
 */

#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include "eii/msgbus/msgbus.h"
#include "publisher.h"

#include "eii/utils/json_config.h"

#define MAX_CONFIG_KEY_LENGTH 40

Publisher::Publisher(std::atomic<bool> *loop) {
    pub_ch = new ConfigMgr();
    this->loop = loop;
}

Publisher::~Publisher() {
    if (m_pub_ctx != NULL)
        msgbus_publisher_destroy(g_msgbus_ctx, m_pub_ctx);
    if (g_msgbus_ctx != NULL)
        msgbus_destroy(g_msgbus_ctx);
}

bool Publisher::init() {

    int num_of_publishers = pub_ch->getNumPublishers();
    if (num_of_publishers == -1) {
        LOG_ERROR_0("No publisher instances found, exiting...");
        return false;
    }

    PublisherCfg* pub_ctx = pub_ch->getPublisherByIndex(0);
    // PublisherCfg* pub_ctx = pub_ch->getPublisherByName("sample_pub");
    config_t* pub_config = pub_ctx->getMsgBusConfig();

    // get all Publisher Topics 
    std::vector<std::string> topics = pub_ctx->getTopics();
    for(int i = 0; i < topics.size(); i++) {
        LOG_INFO("Pub Topics : %s", topics[i].c_str());
    }

    g_msgbus_ctx = msgbus_initialize(pub_config);
    if (g_msgbus_ctx == NULL) {
        LOG_ERROR_0("Failed to initialize message bus");
        goto err;
    }

    msgbus_ret_t ret;
    ret = msgbus_publisher_new(g_msgbus_ctx, topics[0].c_str(), &m_pub_ctx);

    if (ret != MSG_SUCCESS) {
        LOG_ERROR("Failed to initialize publisher (errno: %d)", ret);
        goto err;
    }

    return true;

err:
    if (m_pub_ctx != NULL)
        msgbus_publisher_destroy(g_msgbus_ctx, m_pub_ctx);
    if (g_msgbus_ctx != NULL)
        msgbus_destroy(g_msgbus_ctx);

    return false;
}

void* Publisher::start(void *arg) {
    int ret_val = -1;
    Publisher *obj = reinterpret_cast<Publisher*>(arg);

    if ((ret_val = obj->publish()) != 0) {
        LOG_ERROR("publish failed.(errno: %d)", ret_val);
    }

    delete obj;
}

int Publisher::publish() {
    int ret = 0;
    LOG_INFO_0("Running...");
    int sleep_val = 0;
    unsigned seed = 1;
    try {
        AppCfg* cfg = pub_ch->getAppConfig();
        config_value_t* app_config = cfg->getConfigValue("loop_interval");
        if (app_config->type != CVT_INTEGER) {
            LOG_ERROR_0("loop_interval is not integer");
            exit(1);
        }
        sleep_val = app_config->body.integer;
    }catch(const std::exception& ex) {
        LOG_ERROR("Exception '%s' occurred", ex.what());
    }
    while (loop->load()) {
        LOG_INFO_0("Publishing message");
        // Initialize message to be published
        msg_envelope_elem_body_t* integer;
        integer = msgbus_msg_envelope_new_integer(rand_r(&seed) % 100);
        msg_envelope_elem_body_t* fp;
        fp = msgbus_msg_envelope_new_floating(
             (static_cast<float>(rand_r(&seed))/static_cast<float>(RAND_MAX)));

        g_msg = msgbus_msg_envelope_new(CT_JSON);

        msgbus_msg_envelope_put(g_msg, "hello", integer);
        msgbus_msg_envelope_put(g_msg, "world", fp);
        num_parts = msgbus_msg_envelope_serialize(g_msg, &parts);
        if (num_parts <= 0) {
            LOG_ERROR_0("Failed to serialize message");
            goto err;
        }

        LOG_INFO("Message Published at Publisher: %s", parts[0].bytes)
        ret = msgbus_publisher_publish(g_msgbus_ctx, m_pub_ctx, g_msg);
        if (ret != MSG_SUCCESS) {
            LOG_ERROR("Failed to publish message (errno: %d)", ret);
            goto err;
        }
        sleep(sleep_val);
    }
    return 0;

err:
    if (m_pub_ctx != NULL)
        msgbus_publisher_destroy(g_msgbus_ctx, m_pub_ctx);
    if (g_msgbus_ctx != NULL)
        msgbus_destroy(g_msgbus_ctx);

    return -1;
}
