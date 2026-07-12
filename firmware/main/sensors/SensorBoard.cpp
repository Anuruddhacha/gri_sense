#include "sensors/SensorBoard.hpp"

#include "esp_log.h"

namespace agri::sensors {

static const char* TAG = "SensorBoard";

SensorBoard::SensorBoard(std::unique_ptr<ITempHumiditySensor> temp_humidity,
                         std::unique_ptr<ISoilMoistureSensor> soil_moisture,
                         std::unique_ptr<ILightSensor> light,
                         std::unique_ptr<IPhSensor> ph)
    : temp_humidity_(std::move(temp_humidity)),
      soil_moisture_(std::move(soil_moisture)),
      light_(std::move(light)),
      ph_(std::move(ph))
{
}

bool SensorBoard::init()
{
    bool ok = true;
    ok = temp_humidity_->init() && ok;
    ok = soil_moisture_->init() && ok;
    ok = light_->init() && ok;
    ok = ph_->init() && ok;
    if (!ok) {
        ESP_LOGE(TAG, "one or more sensors failed init");
    }
    return ok;
}

bool SensorBoard::read(AgriTelemetry& out)
{
    AgriTelemetry sample{};
    bool ok = true;

    ok = temp_humidity_->read(sample.temperature_c, sample.humidity_pct) && ok;
    ok = soil_moisture_->read(sample.soil_moisture_pct) && ok;
    ok = light_->read(sample.light_lux) && ok;
    ok = ph_->read(sample.soil_ph) && ok;

    if (ok) {
        out = sample;
    }
    return ok;
}

}  // namespace agri::sensors
