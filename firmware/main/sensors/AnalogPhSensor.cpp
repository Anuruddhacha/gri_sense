#include "sensors/AnalogPhSensor.hpp"

#include "config/AppConfig.hpp"
#include "esp_log.h"
#include "esp_random.h"

namespace agri::sensors {

namespace {
const char* TAG = "SoilPH";
}

AnalogPhSensor::AnalogPhSensor(int adc_gpio)
    : adc_gpio_(adc_gpio)
{
}

bool AnalogPhSensor::init()
{
    ESP_LOGI(TAG, "init on GPIO%d (simulate=%d)", adc_gpio_,
             static_cast<int>(agri::config::kSimulateSensors));
    if (agri::config::kSimulateSensors) {
        return true;
    }
    // TODO: configure ADC + two-point calibration
    return true;
}

bool AnalogPhSensor::read(float& ph)
{
    if (agri::config::kSimulateSensors) {
        return readSimulated(ph);
    }
    return readHardware(ph);
}

bool AnalogPhSensor::readSimulated(float& ph)
{
    ph = 6.2f + static_cast<float>(esp_random() % 10) / 10.0f;
    return true;
}

bool AnalogPhSensor::readHardware(float& ph)
{
    // TODO: read ADC and convert with calibration curve
    (void)adc_gpio_;
    (void)ph;
    ESP_LOGW(TAG, "hardware read not implemented yet");
    return false;
}

}  // namespace agri::sensors
