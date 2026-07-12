#include "sensors/CapacitiveSoilMoisture.hpp"

#include "config/AppConfig.hpp"
#include "esp_log.h"
#include "esp_random.h"

namespace agri::sensors {

namespace {
const char* TAG = "SoilMoisture";
}

CapacitiveSoilMoisture::CapacitiveSoilMoisture(int adc_gpio)
    : adc_gpio_(adc_gpio)
{
}

bool CapacitiveSoilMoisture::init()
{
    ESP_LOGI(TAG, "init on GPIO%d (simulate=%d)", adc_gpio_,
             static_cast<int>(agri::config::kSimulateSensors));
    if (agri::config::kSimulateSensors) {
        return true;
    }
    // TODO: configure ADC oneshot on adc_gpio_
    return true;
}

bool CapacitiveSoilMoisture::read(float& moisture_pct)
{
    if (agri::config::kSimulateSensors) {
        return readSimulated(moisture_pct);
    }
    return readHardware(moisture_pct);
}

bool CapacitiveSoilMoisture::readSimulated(float& moisture_pct)
{
    moisture_pct = 40.0f + static_cast<float>(esp_random() % 150) / 10.0f;
    return true;
}

bool CapacitiveSoilMoisture::readHardware(float& moisture_pct)
{
    // TODO: read ADC, map dry/wet calibration to 0..100%
    (void)adc_gpio_;
    (void)moisture_pct;
    ESP_LOGW(TAG, "hardware read not implemented yet");
    return false;
}

}  // namespace agri::sensors
