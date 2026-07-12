#include "sensors/Dht22TempHumidity.hpp"

#include "config/AppConfig.hpp"
#include "esp_log.h"
#include "esp_random.h"

namespace agri::sensors {

namespace {
const char* TAG = "DHT22";
}

Dht22TempHumidity::Dht22TempHumidity(int gpio_num)
    : gpio_num_(gpio_num)
{
}

bool Dht22TempHumidity::init()
{
    ESP_LOGI(TAG, "init on GPIO%d (simulate=%d)", gpio_num_,
             static_cast<int>(agri::config::kSimulateSensors));
    if (agri::config::kSimulateSensors) {
        return true;
    }
    // TODO: install one-wire / DHT protocol on gpio_num_
    return true;
}

bool Dht22TempHumidity::read(float& temperature_c, float& humidity_pct)
{
    if (agri::config::kSimulateSensors) {
        return readSimulated(temperature_c, humidity_pct);
    }
    return readHardware(temperature_c, humidity_pct);
}

bool Dht22TempHumidity::readSimulated(float& temperature_c, float& humidity_pct)
{
    temperature_c = 27.5f + static_cast<float>(esp_random() % 30) / 10.0f;
    humidity_pct  = 55.0f + static_cast<float>(esp_random() % 100) / 10.0f;
    return true;
}

bool Dht22TempHumidity::readHardware(float& temperature_c, float& humidity_pct)
{
    // TODO: sample DHT22 on gpio_num_ into temperature_c / humidity_pct
    (void)gpio_num_;
    (void)temperature_c;
    (void)humidity_pct;
    ESP_LOGW(TAG, "hardware read not implemented yet");
    return false;
}

}  // namespace agri::sensors
