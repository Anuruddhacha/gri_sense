#pragma once

#include "sensors/AgriTelemetry.hpp"

#include <cstddef>

namespace agri::telemetry {

// Builds compact JSON for the Qt dashboard.
bool toJson(const char* device_id,
            const sensors::AgriTelemetry& data,
            unsigned long uptime_ms,
            char* buffer,
            size_t buffer_len);

}  // namespace agri::telemetry
