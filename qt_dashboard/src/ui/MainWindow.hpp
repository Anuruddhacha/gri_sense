#pragma once

#include "model/TelemetryModel.hpp"
#include "mqtt/MqttSubscriber.hpp"
#include "ui/SensorTile.hpp"

#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
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
    void onEnablePump();
    void onDisablePump();
    void onManualStart();
    void onManualStop();

private:
    void buildUi();
    void applyStyle();
    void sendPumpEnabled(bool enabled);
    void sendPumpCommand(const char* command);
    void updatePumpUi();

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

    QLabel* pumpStateLabel_ = nullptr;
    QLabel* pumpModeLabel_ = nullptr;
    QPushButton* startBtn_ = nullptr;
    QPushButton* stopBtn_ = nullptr;
    QPushButton* enableBtn_ = nullptr;
    QPushButton* disableBtn_ = nullptr;
};

}  // namespace agri::ui
