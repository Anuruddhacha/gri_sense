#include "telemetry/TelemetryJson.hpp"

#include <cstdio>

namespace agri::telemetry {

bool toJson(const char* device_id,
            const sensors::AgriTelemetry& data,
            unsigned long uptime_ms,
            char* buffer,
            size_t buffer_len)
{
    const int n = std::snprintf(
        buffer,
        buffer_len,
        "{"
        "\"device_id\":\"%s\","
        "\"temperature_c\":%.2f,"
        "\"humidity_pct\":%.2f,"
        "\"soil_moisture_pct\":%.2f,"
        "\"light_lux\":%.1f,"
        "\"soil_ph\":%.2f,"
        "\"uptime_ms\":%lu"
        "}",
        device_id,
        static_cast<double>(data.temperature_c),
        static_cast<double>(data.humidity_pct),
        static_cast<double>(data.soil_moisture_pct),
        static_cast<double>(data.light_lux),
        static_cast<double>(data.soil_ph),
        uptime_ms);

    return n > 0 && static_cast<size_t>(n) < buffer_len;
}

}  // namespace agri::telemetry
