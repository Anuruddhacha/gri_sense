#pragma once

#include "model/TelemetryModel.hpp"
#include "mqtt/MqttSubscriber.hpp"
#include "ui/SensorTile.hpp"

#include <QLabel>
#include <QMainWindow>
#include <memory>

namespace agri::ui {

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void onStatus(const QString& text);
    void onConnected(bool connected);
    void onMessage(const QByteArray& payload);
    void onTelemetryChanged();

private:
    void buildUi();
    void applyStyle();

    model::TelemetryModel model_;
    std::unique_ptr<mqtt::MqttSubscriber> mqtt_;

    QLabel* statusLabel_ = nullptr;
    QLabel* deviceLabel_ = nullptr;
    QLabel* updatedLabel_ = nullptr;

    SensorTile* tempTile_ = nullptr;
    SensorTile* humidityTile_ = nullptr;
    SensorTile* moistureTile_ = nullptr;
    SensorTile* lightTile_ = nullptr;
    SensorTile* phTile_ = nullptr;
};

}  // namespace agri::ui
