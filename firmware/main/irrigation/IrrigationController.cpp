#include "irrigation/IrrigationController.hpp"

#include "config/AppConfig.hpp"
#include "esp_log.h"

namespace agri::irrigation {

namespace {
const char* TAG = "Irrigation";
}

IrrigationController::IrrigationController(actuators::WaterPump& pump)
    : pump_(pump), enabled_(agri::config::kPumpEnabledDefault)
{
}

void IrrigationController::setEnabled(bool enabled)
{
    enabled_ = enabled;
    manual_mode_ = false;  // return control to auto logic
    ESP_LOGI(TAG, "auto irrigation %s", enabled_ ? "ENABLED" : "DISABLED");
    if (!enabled_) {
        pump_.setRunning(false);
    }
}

void IrrigationController::manualStart()
{
    manual_mode_ = true;
    pump_.setRunning(true);
    ESP_LOGI(TAG, "manual START");
}

void IrrigationController::manualStop()
{
    manual_mode_ = true;
    pump_.setRunning(false);
    ESP_LOGI(TAG, "manual STOP");
}

bool IrrigationController::isPumpRunning() const
{
    return pump_.isRunning();
}

void IrrigationController::evaluate(const sensors::AgriTelemetry& sample)
{
    if (manual_mode_) {
        return;  // dashboard Start/Stop owns the pump
    }

    if (!enabled_) {
        pump_.setRunning(false);
        return;
    }

    const bool climate_ok =
        sample.temperature_c >= agri::config::kTempMinC &&
        sample.temperature_c <= agri::config::kTempMaxC &&
        sample.humidity_pct <= agri::config::kHumidityMaxPct &&
        sample.light_lux >= agri::config::kLightMinLux;

    if (!climate_ok) {
        if (pump_.isRunning()) {
            ESP_LOGI(TAG, "climate block -> stop pump (T=%.1f H=%.1f L=%.0f)",
                     sample.temperature_c, sample.humidity_pct, sample.light_lux);
        }
        pump_.setRunning(false);
        return;
    }

    if (!pump_.isRunning() && sample.soil_moisture_pct < agri::config::kMoistureOnPct) {
        ESP_LOGI(TAG, "soil dry (%.1f%%) -> start pump", sample.soil_moisture_pct);
        pump_.setRunning(true);
        return;
    }

    if (pump_.isRunning() && sample.soil_moisture_pct >= agri::config::kMoistureOffPct) {
        ESP_LOGI(TAG, "soil wet (%.1f%%) -> stop pump", sample.soil_moisture_pct);
        pump_.setRunning(false);
    }
}

}  // namespace agri::irrigation
