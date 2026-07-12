#include "config/AppConfig.hpp"
#include "mqtt/MqttPublisher.hpp"
#include "sensors/AnalogPhSensor.hpp"
#include "sensors/Bh1750LightSensor.hpp"
#include "sensors/CapacitiveSoilMoisture.hpp"
#include "sensors/Dht22TempHumidity.hpp"
#include "sensors/SensorBoard.hpp"
#include "telemetry/TelemetryJson.hpp"
#include "wifi/WifiStation.hpp"

#include <memory>

#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"

namespace {

const char* TAG = "app";

struct AppContext {
    agri::sensors::SensorBoard* board = nullptr;
    agri::mqtt::MqttPublisher* mqtt = nullptr;
};

void publishTask(void* arg)
{
    auto* ctx = static_cast<AppContext*>(arg);
    char json[256];

    while (true) {
        if (!ctx->mqtt->isConnected()) {
            ESP_LOGW(TAG, "MQTT not connected, skip publish");
            vTaskDelay(pdMS_TO_TICKS(agri::config::kPublishIntervalMs));
            continue;
        }

        agri::sensors::AgriTelemetry sample{};
        if (!ctx->board->read(sample)) {
            ESP_LOGW(TAG, "sensor read failed");
            vTaskDelay(pdMS_TO_TICKS(agri::config::kPublishIntervalMs));
            continue;
        }

        const auto uptime_ms = static_cast<unsigned long>(esp_timer_get_time() / 1000);
        if (!agri::telemetry::toJson(agri::config::kDeviceId, sample, uptime_ms, json, sizeof(json))) {
            ESP_LOGE(TAG, "JSON encode failed");
            vTaskDelay(pdMS_TO_TICKS(agri::config::kPublishIntervalMs));
            continue;
        }

        const int msg_id = ctx->mqtt->publish(agri::config::kMqttTopic, json);
        ESP_LOGI(TAG, "pub id=%d %s", msg_id, json);
        vTaskDelay(pdMS_TO_TICKS(agri::config::kPublishIntervalMs));
    }
}

}  // namespace

extern "C" void app_main(void)
{
    ESP_LOGI(TAG, "AgriSense publisher starting");

    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    agri::wifi::WifiStation wifi;
    if (!wifi.connect(agri::config::kWifiSsid, agri::config::kWifiPassword)) {
        ESP_LOGE(TAG, "Wi-Fi failed — check AppConfig.hpp");
        return;
    }

    // Must outlive publish_task (app_main returns; do not keep these on the stack).
    static agri::sensors::SensorBoard board(
        std::make_unique<agri::sensors::Dht22TempHumidity>(agri::config::kDht22Gpio),
        std::make_unique<agri::sensors::CapacitiveSoilMoisture>(agri::config::kSoilMoistureAdcGpio),
        std::make_unique<agri::sensors::Bh1750LightSensor>(
            agri::config::kBh1750SdaGpio, agri::config::kBh1750SclGpio, agri::config::kBh1750I2cAddr),
        std::make_unique<agri::sensors::AnalogPhSensor>(agri::config::kSoilPhAdcGpio));

    if (!board.init()) {
        ESP_LOGE(TAG, "sensor board init failed");
        return;
    }

    static agri::mqtt::MqttPublisher mqtt;
    if (!mqtt.start(agri::config::kMqttBrokerUri)) {
        ESP_LOGE(TAG, "MQTT start failed");
        return;
    }

    static AppContext ctx{&board, &mqtt};
    xTaskCreate(publishTask, "publish_task", 6144, &ctx, 5, nullptr);
}
