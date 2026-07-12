#pragma once

#include "core/TelemetryData.hpp"

#include <MQTTClient.h>

#include <QObject>
#include <QString>

namespace agri::mqtt {

class MqttSubscriber : public QObject {
    Q_OBJECT

public:
    explicit MqttSubscriber(QObject* parent = nullptr);
    ~MqttSubscriber() override;

    bool connectToBroker(const QString& brokerUri, const QString& clientId);
    bool subscribe(const QString& topic);
    bool publish(const QString& topic, const QByteArray& payload);
    void disconnectFromBroker();
    bool isConnected() const { return connected_; }

signals:
    void connectedChanged(bool connected);
    void messageReceived(const QByteArray& payload);
    void statusMessage(const QString& text);

private:
    static void onConnectionLost(void* context, char* cause);
    static int onMessageArrived(void* context, char* topicName, int topicLen,
                                MQTTClient_message* message);

    MQTTClient client_ = nullptr;
    bool connected_ = false;
};

}  // namespace agri::mqtt
