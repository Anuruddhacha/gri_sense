#pragma once

namespace agri::sensors {

// Shared telemetry snapshot published over MQTT as JSON.
struct AgriTelemetry {
    float temperature_c     = 0.0f;  // DHT22 / SHT3x
    float humidity_pct      = 0.0f;  // DHT22 / SHT3x
    float soil_moisture_pct = 0.0f;  // Capacitive soil moisture
    float light_lux         = 0.0f;  // BH1750
    float soil_ph           = 0.0f;  // Analog soil pH probe
};

}  // namespace agri::sensors
