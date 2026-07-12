#include "mqtt/MqttPublisher.hpp"

#include "esp_log.h"

namespace agri::mqtt {

namespace {
const char* TAG = "MqttPublisher";
}

void MqttPublisher::eventHandler(void* handler_args, esp_event_base_t base, int32_t event_id, void* event_data)
{
    (void)base;
    auto* self = static_cast<MqttPublisher*>(handler_args);
    auto* event = static_cast<esp_mqtt_event_handle_t>(event_data);

    switch (static_cast<esp_mqtt_event_id_t>(event_id)) {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "MQTT connected");
        self->connected_ = true;
        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "MQTT disconnected");
        self->connected_ = false;
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(TAG, "published msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGW(TAG, "MQTT error");
        break;
    default:
        break;
    }
}

bool MqttPublisher::start(const char* broker_uri)
{
    esp_mqtt_client_config_t cfg = {};
    cfg.broker.address.uri = broker_uri;

    client_ = esp_mqtt_client_init(&cfg);
    if (client_ == nullptr) {
        return false;
    }

    esp_mqtt_client_register_event(client_, MQTT_EVENT_ANY, &MqttPublisher::eventHandler, this);
    return esp_mqtt_client_start(client_) == ESP_OK;
}

int MqttPublisher::publish(const char* topic, const char* payload)
{
    if (client_ == nullptr || !connected_) {
        return -1;
    }
    return esp_mqtt_client_publish(client_, topic, payload, 0, 1, 0);
}

}  // namespace agri::mqtt
