#include "sensors/Bh1750LightSensor.hpp"

#include "config/AppConfig.hpp"
#include "esp_log.h"
#include "esp_random.h"

namespace agri::sensors {

namespace {
const char* TAG = "BH1750";
}

Bh1750LightSensor::Bh1750LightSensor(int sda_gpio, int scl_gpio, uint8_t i2c_addr)
    : sda_gpio_(sda_gpio), scl_gpio_(scl_gpio), i2c_addr_(i2c_addr)
{
}

bool Bh1750LightSensor::init()
{
    ESP_LOGI(TAG, "init I2C SDA=%d SCL=%d addr=0x%02X (simulate=%d)",
             sda_gpio_, scl_gpio_, i2c_addr_,
             static_cast<int>(agri::config::kSimulateSensors));
    if (agri::config::kSimulateSensors) {
        return true;
    }
    // TODO: init I2C bus + BH1750 continuous high-res mode
    return true;
}

bool Bh1750LightSensor::read(float& lux)
{
    if (agri::config::kSimulateSensors) {
        return readSimulated(lux);
    }
    return readHardware(lux);
}

bool Bh1750LightSensor::readSimulated(float& lux)
{
    lux = 8000.0f + static_cast<float>(esp_random() % 4000);
    return true;
}

bool Bh1750LightSensor::readHardware(float& lux)
{
    // TODO: I2C read from BH1750 into lux
    (void)sda_gpio_;
    (void)scl_gpio_;
    (void)i2c_addr_;
    (void)lux;
    ESP_LOGW(TAG, "hardware read not implemented yet");
    return false;
}

}  // namespace agri::sensors
