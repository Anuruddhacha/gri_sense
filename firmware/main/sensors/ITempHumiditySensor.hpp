#pragma once

namespace agri::sensors {

// DHT22 / SHT3x — air temperature + relative humidity
class ITempHumiditySensor {
public:
    virtual ~ITempHumiditySensor() = default;
    virtual bool init() = 0;
    virtual bool read(float& temperature_c, float& humidity_pct) = 0;
};

}  // namespace agri::sensors
