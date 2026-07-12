#include "ui/MainWindow.hpp"

#include "config/AppConfig.hpp"
#include "core/TelemetryParser.hpp"

#include <QGridLayout>
#include <QHBoxLayout>
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
    resize(960, 640);

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
        "#tileUnit { color: #7f9f88; font-size: 12px; }"));
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
}

}  // namespace agri::ui
