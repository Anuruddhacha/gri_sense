#pragma once

namespace agri::sensors {

// Capacitive soil moisture probe (ADC)
class ISoilMoistureSensor {
public:
    virtual ~ISoilMoistureSensor() = default;
    virtual bool init() = 0;
    virtual bool read(float& moisture_pct) = 0;
};

}  // namespace agri::sensors
