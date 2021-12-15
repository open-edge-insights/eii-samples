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
 * @brief Publisher main program
 */

#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include "eii/msgbus/msgbus.h"
#include "publisher.h"
#include "server.h"
#include "eii/utils/logger.h"

using namespace std;

pthread_t pub_thread;
pthread_t server_thread;
std::atomic<bool> *loop;
int start_publisher();
int start_server();

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
    } catch(std::exception e){
        LOG_ERROR("Error in setting log level %s", e.what());
    }

    set_log_level(log_level);

    loop = new std::atomic<bool>;
    *loop = true;

    if (start_publisher()  == 0 && start_server() == 0) {
        pthread_join(pub_thread, NULL);
        pthread_join(server_thread, NULL);
    }

    delete loop;
    return 0;
}

int start_publisher() {
    Publisher *pub = new Publisher(loop);
    int ret_val = -1;

    if (pub->init()) {
        LOG_INFO_0("Before thread started..\n");
        if ((ret_val = pthread_create(&pub_thread, NULL, Publisher::start,
            pub)) != 0) {
            LOG_ERROR("Unable to initialize publisher thread, error code : %d",
                      ret_val);
        }
    } else {
        delete pub;
        LOG_ERROR_0("Initializing publisher object failed");
    }

    return ret_val;
}

int start_server() {
    Server *ser = new Server(loop);
    int ret_val = -1;

    if (ser->init()) {
        if ((ret_val = pthread_create(&server_thread, NULL, Server::start,
            ser)) != 0) {
            LOG_ERROR("Unable to initialize server thread, error code : %d",
                      ret_val);
        }
    } else {
            delete ser;
            LOG_ERROR_0("Initializing server object failed");
    }

    return ret_val;
}


