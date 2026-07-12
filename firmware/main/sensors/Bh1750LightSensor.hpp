#pragma once

#include <cstdint>

#include "sensors/ILightSensor.hpp"

namespace agri::sensors {

// BH1750 ambient light sensor (I2C)
class Bh1750LightSensor final : public ILightSensor {
public:
    Bh1750LightSensor(int sda_gpio, int scl_gpio, uint8_t i2c_addr = 0x23);

    bool init() override;
    bool read(float& lux) override;

private:
    bool readSimulated(float& lux);
    bool readHardware(float& lux);

    int sda_gpio_;
    int scl_gpio_;
    uint8_t i2c_addr_;
};

}  // namespace agri::sensors
