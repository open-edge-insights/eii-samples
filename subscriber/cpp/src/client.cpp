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
 * @brief Message bus echo service Client example
 */

#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "eii/msgbus/msgbus.h"
#include "eii/utils/logger.h"
#include "eii/utils/json_config.h"
#include "client.h"

#define MAX_CONFIG_KEY_LENGTH 40
char* cpp_sub_config = NULL;

Client::Client(std::atomic<bool> *loop) {
    client_ch = new ConfigMgr();
    this->loop = loop;
}

Client::~Client() {
    if (msg != NULL)
        msgbus_msg_envelope_destroy(msg);
    if (parts != NULL)
        msgbus_msg_envelope_serialize_destroy(parts, num_parts);
    if (g_service_ctx != NULL)
        msgbus_recv_ctx_destroy(g_msgbus_ctx_client, g_service_ctx);
    if (g_msgbus_ctx_client != NULL)
        msgbus_destroy(g_msgbus_ctx_client);
}

bool Client::init() {

    int num_of_clients = client_ch->getNumClients();
    if (num_of_clients == -1) {
        LOG_ERROR_0("No client instances found, exiting...");
        return false;
    }

    // ClientCfg* client_ctx = client_ch->getClientByName("default");
    ClientCfg* client_ctx = client_ch->getClientByIndex(0);
    config_t* config_client = client_ctx->getMsgBusConfig();

    g_msgbus_ctx_client = msgbus_initialize(config_client);
    if (g_msgbus_ctx_client == NULL) {
        LOG_ERROR_0("Failed to initialize message bus");
        goto err;
    }

    msgbus_ret_t ret;
    ret = msgbus_service_get(g_msgbus_ctx_client, "default", NULL,
                             &g_service_ctx);
    if (ret != MSG_SUCCESS) {
        LOG_ERROR("Failed to initialize service (errno: %d)", ret);
        goto err;
    }

    return true;
err:
    if (g_service_ctx != NULL)
        msgbus_recv_ctx_destroy(g_msgbus_ctx_client, g_service_ctx);
    if (g_msgbus_ctx_client != NULL)
        msgbus_destroy(g_msgbus_ctx_client);

    return false;
}

void* Client::start(void *arg) {
    int ret_val = -1;
    Client *obj = reinterpret_cast<Client*>(arg);

    if ((ret_val = obj -> client_service()) != 0) {
        LOG_ERROR("client service failed.(errno: %d)", ret_val);
    }
    delete obj;
}

int Client::client_service() {
    int ret = 0;
    int sleep_val = 0;
    unsigned seed = 1;
    try {
        AppCfg* cfg = client_ch->getAppConfig();
        config_value_t* app_config = cfg->getConfigValue("loop_interval");
        if (app_config->type != CVT_INTEGER) {
            LOG_ERROR_0("loop_interval is not integer");
            exit(1);
        }
        sleep_val = app_config->body.integer;
    }catch(const std::exception& ex) {
        LOG_ERROR("Exception '%s' occurred", ex.what());
    }

    while (*loop) {
        // Initailize request
        msg_envelope_elem_body_t* integer;
        integer = msgbus_msg_envelope_new_integer(rand_r(&seed) % 100);
        msg_envelope_elem_body_t* fp;
        fp = msgbus_msg_envelope_new_floating(
             static_cast<float>(rand_r(&seed))/static_cast<float>(RAND_MAX));

        msg = msgbus_msg_envelope_new(CT_JSON);

        msgbus_msg_envelope_put(msg, "hello", integer);
        msgbus_msg_envelope_put(msg, "world", fp);

        LOG_INFO_0("Sending request");
        num_parts = msgbus_msg_envelope_serialize(msg, &parts);
        if (num_parts <= 0) {
            LOG_ERROR_0("Failed to serialize message");
            goto err;
        }

        LOG_INFO("Message sent from client: %s", parts[0].bytes);
        ret = msgbus_request(g_msgbus_ctx_client, g_service_ctx, msg);
        if (ret != MSG_SUCCESS) {
            LOG_ERROR("Failed to send request (errno: %d)", ret);
            goto err;
        }
        sleep(sleep_val);
        msgbus_msg_envelope_destroy(msg);
        msg = NULL;

        LOG_INFO_0("Waiting for response");
        ret = msgbus_recv_wait(g_msgbus_ctx_client, g_service_ctx, &msg);
        if (ret != MSG_SUCCESS) {
            // Interrupt is an acceptable error
            if (ret != MSG_ERR_EINTR)
                LOG_ERROR("Failed to receive message (errno: %d)", ret);
            goto err;
        }

        num_parts = msgbus_msg_envelope_serialize(msg, &parts);
        if (num_parts <= 0) {
            LOG_ERROR_0("Failed to serialize message");
            goto err;
        }

        LOG_INFO("Received Response back at Client: %s", parts[0].bytes);
        msgbus_msg_envelope_serialize_destroy(parts, num_parts);
        msgbus_msg_envelope_destroy(msg);
        msg = NULL;
    }
    msgbus_recv_ctx_destroy(g_msgbus_ctx_client, g_service_ctx);
    msgbus_destroy(g_msgbus_ctx_client);

    return 0;

err:
    if (msg != NULL)
        msgbus_msg_envelope_destroy(msg);
    if (parts != NULL)
        msgbus_msg_envelope_serialize_destroy(parts, num_parts);
    if (g_service_ctx != NULL)
        msgbus_recv_ctx_destroy(g_msgbus_ctx_client, g_service_ctx);
    if (g_msgbus_ctx_client != NULL)
        msgbus_destroy(g_msgbus_ctx_client);
    return -1;
}
