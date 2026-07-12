#pragma once

#include "mqtt_client.h"

namespace agri::mqtt {

class MqttPublisher {
public:
    bool start(const char* broker_uri);
    bool isConnected() const { return connected_; }
    int publish(const char* topic, const char* payload);

private:
    static void eventHandler(void* handler_args, esp_event_base_t base, int32_t event_id, void* event_data);

    esp_mqtt_client_handle_t client_ = nullptr;
    bool connected_ = false;
};

}  // namespace agri::mqtt
