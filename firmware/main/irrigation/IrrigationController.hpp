#pragma once

#include "actuators/WaterPump.hpp"
#include "sensors/AgriTelemetry.hpp"

namespace agri::irrigation {

class IrrigationController {
public:
    explicit IrrigationController(actuators::WaterPump& pump);

    void setEnabled(bool enabled);
    void manualStart();
    void manualStop();

    bool isEnabled() const { return enabled_; }
    bool isManualMode() const { return manual_mode_; }
    bool isPumpRunning() const;

    // Decide pump on/off from latest sensor sample (skipped in manual mode).
    void evaluate(const sensors::AgriTelemetry& sample);

private:
    actuators::WaterPump& pump_;
    bool enabled_ = true;
    bool manual_mode_ = false;
};

}  // namespace agri::irrigation
