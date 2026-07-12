#include "ui/MainWindow.hpp"

#include "config/AppConfig.hpp"
#include "core/TelemetryParser.hpp"

#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QStyle>
#include <QWidget>
#include <QVBoxLayout>

namespace agri::ui {

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    buildUi();
    applyStyle();

    connect(&model_, &model::TelemetryModel::telemetryChanged, this, &MainWindow::onTelemetryChanged);

    mqtt_ = std::make_unique<mqtt::MqttSubscriber>(this);
    connect(mqtt_.get(), &mqtt::MqttSubscriber::statusMessage, this, &MainWindow::onStatus);
    connect(mqtt_.get(), &mqtt::MqttSubscriber::connectedChanged, this, &MainWindow::onConnected);
    connect(mqtt_.get(), &mqtt::MqttSubscriber::messageReceived, this, &MainWindow::onMessage);

    if (mqtt_->connectToBroker(QString::fromUtf8(agri::config::kBrokerUri),
                               QString::fromUtf8(agri::config::kClientId))) {
        mqtt_->subscribe(QString::fromUtf8(agri::config::kTopic));
    }
}

void MainWindow::buildUi()
{
    auto* central = new QWidget(this);
    setCentralWidget(central);
    setWindowTitle(QStringLiteral("AgriSense Dashboard"));
    resize(980, 720);

    auto* root = new QVBoxLayout(central);
    root->setContentsMargins(28, 24, 28, 24);
    root->setSpacing(18);

    auto* header = new QHBoxLayout();
    auto* brand = new QLabel(QStringLiteral("AgriSense"), this);
    brand->setObjectName(QStringLiteral("brand"));

    deviceLabel_ = new QLabel(QStringLiteral("Device: --"), this);
    deviceLabel_->setObjectName(QStringLiteral("meta"));
    updatedLabel_ = new QLabel(QStringLiteral("Updated: --"), this);
    updatedLabel_->setObjectName(QStringLiteral("meta"));

    auto* metaCol = new QVBoxLayout();
    metaCol->setSpacing(2);
    metaCol->addWidget(deviceLabel_, 0, Qt::AlignRight);
    metaCol->addWidget(updatedLabel_, 0, Qt::AlignRight);

    header->addWidget(brand);
    header->addStretch();
    header->addLayout(metaCol);
    root->addLayout(header);

    auto* grid = new QGridLayout();
    grid->setHorizontalSpacing(16);
    grid->setVerticalSpacing(16);

    tempTile_ = new SensorTile(QStringLiteral("Air Temperature"), QStringLiteral("°C"), this);
    humidityTile_ = new SensorTile(QStringLiteral("Air Humidity"), QStringLiteral("% RH"), this);
    moistureTile_ = new SensorTile(QStringLiteral("Soil Moisture"), QStringLiteral("%"), this);
    lightTile_ = new SensorTile(QStringLiteral("Light (BH1750)"), QStringLiteral("lux"), this);
    phTile_ = new SensorTile(QStringLiteral("Soil pH"), QStringLiteral("pH"), this);

    grid->addWidget(tempTile_, 0, 0);
    grid->addWidget(humidityTile_, 0, 1);
    grid->addWidget(moistureTile_, 0, 2);
    grid->addWidget(lightTile_, 1, 0);
    grid->addWidget(phTile_, 1, 1);
    root->addLayout(grid, 1);

    auto* pumpPanel = new QFrame(this);
    pumpPanel->setObjectName(QStringLiteral("pumpPanel"));
    auto* pumpLayout = new QHBoxLayout(pumpPanel);
    pumpLayout->setContentsMargins(20, 16, 20, 16);
    pumpLayout->setSpacing(16);

    auto* pumpText = new QVBoxLayout();
    auto* pumpTitle = new QLabel(QStringLiteral("Water Pump"), pumpPanel);
    pumpTitle->setObjectName(QStringLiteral("pumpTitle"));
    pumpStateLabel_ = new QLabel(QStringLiteral("Motor: --"), pumpPanel);
    pumpStateLabel_->setObjectName(QStringLiteral("pumpState"));
    pumpModeLabel_ = new QLabel(QStringLiteral("Auto irrigation: --"), pumpPanel);
    pumpModeLabel_->setObjectName(QStringLiteral("pumpMode"));
    pumpText->addWidget(pumpTitle);
    pumpText->addWidget(pumpStateLabel_);
    pumpText->addWidget(pumpModeLabel_);
    pumpLayout->addLayout(pumpText, 1);

    enableBtn_ = new QPushButton(QStringLiteral("Enable"), pumpPanel);
    disableBtn_ = new QPushButton(QStringLiteral("Disable"), pumpPanel);
    enableBtn_->setObjectName(QStringLiteral("enableBtn"));
    disableBtn_->setObjectName(QStringLiteral("disableBtn"));
    enableBtn_->setCursor(Qt::PointingHandCursor);
    disableBtn_->setCursor(Qt::PointingHandCursor);
    connect(enableBtn_, &QPushButton::clicked, this, &MainWindow::onEnablePump);
    connect(disableBtn_, &QPushButton::clicked, this, &MainWindow::onDisablePump);

    pumpLayout->addWidget(enableBtn_);
    pumpLayout->addWidget(disableBtn_);
    root->addWidget(pumpPanel);

    statusLabel_ = new QLabel(QStringLiteral("Starting..."), this);
    statusLabel_->setObjectName(QStringLiteral("status"));
    root->addWidget(statusLabel_);
}

void MainWindow::applyStyle()
{
    setStyleSheet(QStringLiteral(
        "QMainWindow, QWidget { background-color: #122018; color: #e7f0e8; }"
        "#brand { font-size: 34px; font-weight: 700; color: #d8f3dc; letter-spacing: 0.5px; }"
        "#meta { color: #9bb5a2; font-size: 13px; }"
        "#status { color: #8fa896; font-size: 12px; padding-top: 6px; }"
        "#sensorTile { background: qlineargradient(x1:0,y1:0,x2:1,y2:1,"
        "  stop:0 #1b3326, stop:1 #163024); border: 1px solid #2f5a40; border-radius: 14px; }"
        "#tileTitle { color: #a7c4b0; font-size: 13px; }"
        "#tileValue { color: #f4fff6; font-size: 40px; font-weight: 650; }"
        "#tileUnit { color: #7f9f88; font-size: 12px; }"
        "#pumpPanel { background: #183226; border: 1px solid #3a6b4c; border-radius: 14px; }"
        "#pumpTitle { font-size: 16px; font-weight: 600; color: #d8f3dc; }"
        "#pumpState { font-size: 22px; font-weight: 650; }"
        "#pumpMode { color: #9bb5a2; font-size: 13px; }"
        "#enableBtn, #disableBtn { min-width: 110px; padding: 10px 16px; border-radius: 8px;"
        "  border: 1px solid #3a6b4c; background: #214533; color: #e7f0e8; font-weight: 600; }"
        "#enableBtn:hover { background: #2a5a40; }"
        "#disableBtn:hover { background: #5a322a; border-color: #8a4a3a; }"
        "#pumpState[running=\"true\"] { color: #7CFF9A; }"
        "#pumpState[running=\"false\"] { color: #FFB4A2; }"));
}

void MainWindow::onStatus(const QString& text)
{
    statusLabel_->setText(text);
}

void MainWindow::onConnected(bool connected)
{
    if (!connected) {
        statusLabel_->setText(QStringLiteral("Disconnected"));
    }
}

void MainWindow::onMessage(const QByteArray& payload)
{
    const auto parsed = core::TelemetryParser::fromJson(payload);
    if (!parsed.has_value() || !parsed->valid) {
        statusLabel_->setText(QStringLiteral("Invalid telemetry payload"));
        return;
    }
    model_.update(*parsed);
}

void MainWindow::onTelemetryChanged()
{
    deviceLabel_->setText(QStringLiteral("Device: %1").arg(model_.deviceId()));
    updatedLabel_->setText(QStringLiteral("Updated: %1").arg(model_.lastUpdateText()));

    tempTile_->setValue(model_.temperatureC(), 1);
    humidityTile_->setValue(model_.humidityPct(), 1);
    moistureTile_->setValue(model_.soilMoisturePct(), 1);
    lightTile_->setValue(model_.lightLux(), 0);
    phTile_->setValue(model_.soilPh(), 2);
    updatePumpUi();
}

void MainWindow::updatePumpUi()
{
    const bool running = model_.pumpRunning();
    pumpStateLabel_->setText(running ? QStringLiteral("Motor: RUNNING")
                                     : QStringLiteral("Motor: STOPPED"));
    pumpStateLabel_->setProperty("running", running);
    pumpStateLabel_->style()->unpolish(pumpStateLabel_);
    pumpStateLabel_->style()->polish(pumpStateLabel_);

    pumpModeLabel_->setText(model_.pumpEnabled()
                                ? QStringLiteral("Auto irrigation: ENABLED")
                                : QStringLiteral("Auto irrigation: DISABLED"));
}

void MainWindow::sendPumpEnabled(bool enabled)
{
    const QByteArray payload = enabled ? QByteArrayLiteral("{\"pump_enabled\":true}")
                                       : QByteArrayLiteral("{\"pump_enabled\":false}");
    if (mqtt_ && mqtt_->publish(QString::fromUtf8(agri::config::kCommandTopic), payload)) {
        statusLabel_->setText(enabled ? QStringLiteral("Sent: enable water supply")
                                      : QStringLiteral("Sent: disable water supply"));
    }
}

void MainWindow::onEnablePump()
{
    sendPumpEnabled(true);
}

void MainWindow::onDisablePump()
{
    sendPumpEnabled(false);
}

}  // namespace agri::ui
