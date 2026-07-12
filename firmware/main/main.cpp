#include "actuators/WaterPump.hpp"
#include "config/AppConfig.hpp"
#include "irrigation/IrrigationController.hpp"
#include "mqtt/MqttPublisher.hpp"
#include "sensors/AnalogPhSensor.hpp"
#include "sensors/Bh1750LightSensor.hpp"
#include "sensors/CapacitiveSoilMoisture.hpp"
#include "sensors/Dht22TempHumidity.hpp"
#include "sensors/SensorBoard.hpp"
#include "telemetry/TelemetryJson.hpp"
#include "wifi/WifiStation.hpp"

#include <cstring>
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
    agri::irrigation::IrrigationController* irrigation = nullptr;
};

void handleCommand(const char* topic, int topic_len, const char* data, int data_len, void* ctx)
{
    (void)topic;
    (void)topic_len;
    auto* irrigation = static_cast<agri::irrigation::IrrigationController*>(ctx);
    if (irrigation == nullptr || data == nullptr || data_len <= 0) {
        return;
    }

    char cmd[160];
    const int n = data_len < static_cast<int>(sizeof(cmd) - 1) ? data_len : static_cast<int>(sizeof(cmd) - 1);
    std::memcpy(cmd, data, static_cast<size_t>(n));
    cmd[n] = '\0';

    // Accept:
    //   {"pump_enabled":true|false}  auto irrigation
    //   {"pump_command":"start|stop"} manual motor control
    if (std::strstr(cmd, "\"pump_command\":\"start\"") != nullptr ||
        std::strstr(cmd, "\"pump_command\": \"start\"") != nullptr) {
        irrigation->manualStart();
        return;
    }
    if (std::strstr(cmd, "\"pump_command\":\"stop\"") != nullptr ||
        std::strstr(cmd, "\"pump_command\": \"stop\"") != nullptr) {
        irrigation->manualStop();
        return;
    }
    if (std::strstr(cmd, "\"pump_enabled\":true") != nullptr ||
        std::strstr(cmd, "\"pump_enabled\": true") != nullptr) {
        irrigation->setEnabled(true);
        return;
    }
    if (std::strstr(cmd, "\"pump_enabled\":false") != nullptr ||
        std::strstr(cmd, "\"pump_enabled\": false") != nullptr) {
        irrigation->setEnabled(false);
    }
}

void publishTask(void* arg)
{
    auto* ctx = static_cast<AppContext*>(arg);
    char json[384];

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

        ctx->irrigation->evaluate(sample);

        agri::telemetry::PumpStatus pump{
            ctx->irrigation->isEnabled(),
            ctx->irrigation->isPumpRunning(),
            ctx->irrigation->isManualMode(),
        };

        const auto uptime_ms = static_cast<unsigned long>(esp_timer_get_time() / 1000);
        if (!agri::telemetry::toJson(agri::config::kDeviceId, sample, pump, uptime_ms, json, sizeof(json))) {
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

    static agri::actuators::WaterPump pump(agri::config::kWaterPumpGpio);
    if (!pump.init()) {
        ESP_LOGE(TAG, "water pump init failed");
        return;
    }

    static agri::irrigation::IrrigationController irrigation(pump);

    static agri::mqtt::MqttPublisher mqtt;
    mqtt.setCommandCallback(&handleCommand, &irrigation);
    if (!mqtt.start(agri::config::kMqttBrokerUri, agri::config::kMqttCommandTopic)) {
        ESP_LOGE(TAG, "MQTT start failed");
        return;
    }

    static AppContext ctx{&board, &mqtt, &irrigation};
    xTaskCreate(publishTask, "publish_task", 6144, &ctx, 5, nullptr);
}
