#pragma once

#include <QString>

namespace agri::core {

struct TelemetryData {
    QString deviceId;
    double temperatureC = 0.0;
    double humidityPct = 0.0;
    double soilMoisturePct = 0.0;
    double lightLux = 0.0;
    double soilPh = 0.0;
    bool pumpEnabled = false;
    bool pumpRunning = false;
    bool pumpManual = false;
    qint64 uptimeMs = 0;
    bool valid = false;
};

}  // namespace agri::core
