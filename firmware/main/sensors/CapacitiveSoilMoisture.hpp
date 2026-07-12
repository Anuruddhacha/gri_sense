#pragma once

#include "sensors/ISoilMoistureSensor.hpp"

namespace agri::sensors {

// Capacitive soil moisture probe (analog ADC)
class CapacitiveSoilMoisture final : public ISoilMoistureSensor {
public:
    explicit CapacitiveSoilMoisture(int adc_gpio);

    bool init() override;
    bool read(float& moisture_pct) override;

private:
    bool readSimulated(float& moisture_pct);
    bool readHardware(float& moisture_pct);

    int adc_gpio_;
};

}  // namespace agri::sensors
