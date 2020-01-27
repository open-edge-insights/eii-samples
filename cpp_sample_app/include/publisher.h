#include <signal.h>
#include <unistd.h>
#include <eis/config_manager/env_config.h>
#include "eis/msgbus/msgbus.h"
#include <atomic>

#ifndef _EIS_CPP_PUBLISHER_H
#define _EIS_CPP_PUBLISHER_H

class publisher
{
 private:
    publisher_ctx_t* g_pub_ctx = NULL;
    int num_parts = 0;
    void* g_msgbus_ctx = NULL;
    env_config_t* g_env_config_client = NULL;
    config_mgr_t* g_config_mgr = NULL;
    msg_envelope_serialized_part_t* parts = NULL;
    msg_envelope_t* g_msg = NULL;
    std::atomic<bool> *loop;

 public:
    publisher(std::atomic<bool> *loop);
    ~publisher();
    bool init(char *topic_name);
    static void* start(void *arg);
    int publish();
};

#endif //_EIS_CPP_PUBLISHER_H


