#pragma once

#include "core/TelemetryData.hpp"

#include <QObject>
#include <QString>

namespace agri::model {

class TelemetryModel : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString deviceId READ deviceId NOTIFY telemetryChanged)
    Q_PROPERTY(double temperatureC READ temperatureC NOTIFY telemetryChanged)
    Q_PROPERTY(double humidityPct READ humidityPct NOTIFY telemetryChanged)
    Q_PROPERTY(double soilMoisturePct READ soilMoisturePct NOTIFY telemetryChanged)
    Q_PROPERTY(double lightLux READ lightLux NOTIFY telemetryChanged)
    Q_PROPERTY(double soilPh READ soilPh NOTIFY telemetryChanged)
    Q_PROPERTY(bool pumpEnabled READ pumpEnabled NOTIFY telemetryChanged)
    Q_PROPERTY(bool pumpRunning READ pumpRunning NOTIFY telemetryChanged)
    Q_PROPERTY(bool pumpManual READ pumpManual NOTIFY telemetryChanged)
    Q_PROPERTY(bool hasData READ hasData NOTIFY telemetryChanged)
    Q_PROPERTY(QString lastUpdateText READ lastUpdateText NOTIFY telemetryChanged)

public:
    explicit TelemetryModel(QObject* parent = nullptr);

    void update(const core::TelemetryData& data);

    QString deviceId() const { return data_.deviceId; }
    double temperatureC() const { return data_.temperatureC; }
    double humidityPct() const { return data_.humidityPct; }
    double soilMoisturePct() const { return data_.soilMoisturePct; }
    double lightLux() const { return data_.lightLux; }
    double soilPh() const { return data_.soilPh; }
    bool pumpEnabled() const { return data_.pumpEnabled; }
    bool pumpRunning() const { return data_.pumpRunning; }
    bool pumpManual() const { return data_.pumpManual; }
    bool hasData() const { return data_.valid; }
    QString lastUpdateText() const { return lastUpdateText_; }

signals:
    void telemetryChanged();

private:
    core::TelemetryData data_;
    QString lastUpdateText_ = QStringLiteral("Waiting for ESP32...");
};

}  // namespace agri::model
