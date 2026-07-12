#pragma once

#include "mqtt_client.h"

namespace agri::mqtt {

using CommandCallback = void (*)(const char* topic, int topic_len, const char* data, int data_len, void* ctx);

class MqttPublisher {
public:
    bool start(const char* broker_uri, const char* command_topic);
    bool isConnected() const { return connected_; }
    int publish(const char* topic, const char* payload);

    void setCommandCallback(CommandCallback cb, void* ctx);

private:
    static void eventHandler(void* handler_args, esp_event_base_t base, int32_t event_id, void* event_data);

    esp_mqtt_client_handle_t client_ = nullptr;
    bool connected_ = false;
    const char* command_topic_ = nullptr;
    CommandCallback command_cb_ = nullptr;
    void* command_ctx_ = nullptr;
};

}  // namespace agri::mqtt
