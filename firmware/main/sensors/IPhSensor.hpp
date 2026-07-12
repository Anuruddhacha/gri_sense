#pragma once

namespace agri::sensors {

// Analog soil pH probe (ADC + module)
class IPhSensor {
public:
    virtual ~IPhSensor() = default;
    virtual bool init() = 0;
    virtual bool read(float& ph) = 0;
};

}  // namespace agri::sensors
