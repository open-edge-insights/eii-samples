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

/**
 * @file
 * @brief subscriber main program
 */

#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include "eii/msgbus/msgbus.h"
#include "subscriber.h"
#include "client.h"
#include "eii/utils/logger.h"

using namespace std;

pthread_t sub_thread;
pthread_t client_thread;
std::atomic<bool> *loop;
int start_subscriber();
int start_client();

void signal_handler(int signo) {
    loop->store(false, std::memory_order_relaxed);}

int main() {
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    // setting the LOG LEVEL
    const char* str_log_level = NULL;

    // default log level is `INFO`
    log_lvl_t log_level = LOG_LVL_INFO;
    set_log_level(log_level);

    try {
        str_log_level = getenv("C_LOG_LEVEL");
        if (str_log_level == NULL) {
            LOG_ERROR_0("C_LOG_LEVEL env not set");
        } else {
            if (strncmp(str_log_level, "DEBUG", 5) == 0) {
                log_level = LOG_LVL_DEBUG;
            } else if (strncmp(str_log_level, "INFO", 5) == 0) {
                log_level = LOG_LVL_INFO;
            } else if (strncmp(str_log_level, "WARN", 5) == 0) {
                log_level = LOG_LVL_WARN;
            } else if (strncmp(str_log_level, "ERROR", 5) == 0) {
                log_level = LOG_LVL_ERROR;
            }
        }
    } catch(std::exception e) {
        LOG_ERROR("Error in setting log level %s", e.what());
    }

    set_log_level(log_level);

    loop = new std::atomic<bool>;
    *loop = true;
    if ((start_subscriber() == 0) &&
        (start_client() == 0)) {
        pthread_join(sub_thread, NULL);
        pthread_join(client_thread, NULL);
    }

    delete loop;
    return 0;
}

int start_subscriber() {
    Subscriber *sub = new Subscriber(loop);
    int ret_val = -1;

    if (sub->init()) {
    if ((ret_val = pthread_create(&sub_thread, NULL, Subscriber::start,
        sub)) != 0) {
        LOG_ERROR("Unable to initialize subscriber thread, error code : %d",
                  ret_val);
    }
    } else {
        delete sub;
        LOG_ERROR_0("Initializing subscriber object failed");
    }
    return ret_val;
}

int start_client() {
    Client *cli = new Client(loop);
    int ret_val = -1;

    if (cli->init()) {
        if ((ret_val = pthread_create(&client_thread, NULL, Client::start,
            cli)) !=0) {
            LOG_ERROR("Unable to initialize client thread, error code : %d",
                      ret_val);
    }
    } else {
        delete cli;
        LOG_ERROR_0("Initializing client object failed");
    }
    return ret_val;
}
