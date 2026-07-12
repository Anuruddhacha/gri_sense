#pragma once

#include "actuators/WaterPump.hpp"
#include "sensors/AgriTelemetry.hpp"

namespace agri::irrigation {

class IrrigationController {
public:
    explicit IrrigationController(actuators::WaterPump& pump);

    void setEnabled(bool enabled);
    bool isEnabled() const { return enabled_; }
    bool isPumpRunning() const;

    // Decide pump on/off from latest sensor sample.
    void evaluate(const sensors::AgriTelemetry& sample);

private:
    actuators::WaterPump& pump_;
    bool enabled_ = true;
};

}  // namespace agri::irrigation
