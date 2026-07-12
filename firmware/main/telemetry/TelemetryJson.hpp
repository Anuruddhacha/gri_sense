#pragma once

#include "sensors/AgriTelemetry.hpp"

#include <cstddef>

namespace agri::telemetry {

struct PumpStatus {
    bool enabled = false;
    bool running = false;
};

// Builds compact JSON for the Qt dashboard.
bool toJson(const char* device_id,
            const sensors::AgriTelemetry& data,
            const PumpStatus& pump,
            unsigned long uptime_ms,
            char* buffer,
            size_t buffer_len);

}  // namespace agri::telemetry
