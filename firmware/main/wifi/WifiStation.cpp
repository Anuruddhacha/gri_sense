#include "wifi/WifiStation.hpp"

#include <cstring>

#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

namespace agri::wifi {

namespace {
constexpr int kWifiConnectedBit = BIT0;
constexpr int kWifiFailBit      = BIT1;
constexpr int kMaxRetry         = 10;

const char* TAG = "WifiStation";
EventGroupHandle_t s_event_group = nullptr;
int s_retry_num = 0;

void eventHandler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    (void)arg;
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        if (s_retry_num < kMaxRetry) {
            esp_wifi_connect();
            ++s_retry_num;
            ESP_LOGI(TAG, "retry connect to AP");
        } else {
            xEventGroupSetBits(s_event_group, kWifiFailBit);
        }
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        auto* event = static_cast<ip_event_got_ip_t*>(event_data);
        ESP_LOGI(TAG, "got ip: " IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
        xEventGroupSetBits(s_event_group, kWifiConnectedBit);
    }
}
}  // namespace

bool WifiStation::connect(const char* ssid, const char* password)
{
    s_event_group = xEventGroupCreate();
    s_retry_num = 0;

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &eventHandler, nullptr));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &eventHandler, nullptr));

    wifi_config_t wifi_config = {};
    std::strncpy(reinterpret_cast<char*>(wifi_config.sta.ssid), ssid, sizeof(wifi_config.sta.ssid) - 1);
    std::strncpy(reinterpret_cast<char*>(wifi_config.sta.password), password,
                 sizeof(wifi_config.sta.password) - 1);
    wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "connecting to SSID:%s", ssid);

    const EventBits_t bits = xEventGroupWaitBits(
        s_event_group, kWifiConnectedBit | kWifiFailBit, pdFALSE, pdFALSE, portMAX_DELAY);

    if (bits & kWifiConnectedBit) {
        ESP_LOGI(TAG, "connected to SSID:%s", ssid);
        return true;
    }

    ESP_LOGE(TAG, "failed to connect to SSID:%s", ssid);
    return false;
}

}  // namespace agri::wifi
