#pragma once

#include "sensors/IPhSensor.hpp"

namespace agri::sensors {

// Analog soil pH probe / module (ADC)
class AnalogPhSensor final : public IPhSensor {
public:
    explicit AnalogPhSensor(int adc_gpio);

    bool init() override;
    bool read(float& ph) override;

private:
    bool readSimulated(float& ph);
    bool readHardware(float& ph);

    int adc_gpio_;
};

}  // namespace agri::sensors
