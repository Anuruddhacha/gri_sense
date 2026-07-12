#pragma once

#include "sensors/ITempHumiditySensor.hpp"

namespace agri::sensors {

// DHT22 / AM2302 — air temperature + relative humidity
class Dht22TempHumidity final : public ITempHumiditySensor {
public:
    explicit Dht22TempHumidity(int gpio_num);

    bool init() override;
    bool read(float& temperature_c, float& humidity_pct) override;

private:
    bool readSimulated(float& temperature_c, float& humidity_pct);
    bool readHardware(float& temperature_c, float& humidity_pct);

    int gpio_num_;
};

}  // namespace agri::sensors
