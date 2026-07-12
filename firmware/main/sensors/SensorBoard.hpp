#pragma once

#include "sensors/AgriTelemetry.hpp"
#include "sensors/ILightSensor.hpp"
#include "sensors/IPhSensor.hpp"
#include "sensors/ISoilMoistureSensor.hpp"
#include "sensors/ITempHumiditySensor.hpp"

#include <memory>

namespace agri::sensors {

// Composes agriculture drivers. Swap hardcoded impls for real drivers in main.
class SensorBoard {
public:
    SensorBoard(std::unique_ptr<ITempHumiditySensor> temp_humidity,
                std::unique_ptr<ISoilMoistureSensor> soil_moisture,
                std::unique_ptr<ILightSensor> light,
                std::unique_ptr<IPhSensor> ph);

    bool init();
    bool read(AgriTelemetry& out);

private:
    std::unique_ptr<ITempHumiditySensor> temp_humidity_;
    std::unique_ptr<ISoilMoistureSensor> soil_moisture_;
    std::unique_ptr<ILightSensor> light_;
    std::unique_ptr<IPhSensor> ph_;
};

}  // namespace agri::sensors
