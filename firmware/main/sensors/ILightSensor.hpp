#pragma once

namespace agri::sensors {

// BH1750 ambient light sensor (I2C)
class ILightSensor {
public:
    virtual ~ILightSensor() = default;
    virtual bool init() = 0;
    virtual bool read(float& lux) = 0;
};

}  // namespace agri::sensors
