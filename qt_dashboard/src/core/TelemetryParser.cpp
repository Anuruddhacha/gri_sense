#include "core/TelemetryParser.hpp"

#include <QJsonDocument>
#include <QJsonObject>

namespace agri::core {

std::optional<TelemetryData> TelemetryParser::fromJson(const QByteArray& payload)
{
    const QJsonDocument doc = QJsonDocument::fromJson(payload);
    if (!doc.isObject()) {
        return std::nullopt;
    }

    const QJsonObject obj = doc.object();
    TelemetryData data;
    data.deviceId = obj.value(QStringLiteral("device_id")).toString();
    data.temperatureC = obj.value(QStringLiteral("temperature_c")).toDouble();
    data.humidityPct = obj.value(QStringLiteral("humidity_pct")).toDouble();
    data.soilMoisturePct = obj.value(QStringLiteral("soil_moisture_pct")).toDouble();
    data.lightLux = obj.value(QStringLiteral("light_lux")).toDouble();
    data.soilPh = obj.value(QStringLiteral("soil_ph")).toDouble();
    data.pumpEnabled = obj.value(QStringLiteral("pump_enabled")).toBool();
    data.pumpRunning = obj.value(QStringLiteral("pump_running")).toBool();
    data.pumpManual = obj.value(QStringLiteral("pump_manual")).toBool();
    data.uptimeMs = static_cast<qint64>(obj.value(QStringLiteral("uptime_ms")).toDouble());
    data.valid = !data.deviceId.isEmpty();
    return data;
}

}  // namespace agri::core
