#include "mqtt/MqttSubscriber.hpp"

#include <QMetaObject>

namespace agri::mqtt {

MqttSubscriber::MqttSubscriber(QObject* parent)
    : QObject(parent)
{
}

MqttSubscriber::~MqttSubscriber()
{
    disconnectFromBroker();
}

void MqttSubscriber::onConnectionLost(void* context, char* cause)
{
    auto* self = static_cast<MqttSubscriber*>(context);
    self->connected_ = false;
    const QString reason = cause ? QString::fromUtf8(cause) : QStringLiteral("unknown");
    QMetaObject::invokeMethod(
        self,
        [self, reason]() {
            emit self->connectedChanged(false);
            emit self->statusMessage(QStringLiteral("Connection lost: %1").arg(reason));
        },
        Qt::QueuedConnection);
}

int MqttSubscriber::onMessageArrived(void* context, char* topicName, int topicLen,
                                     MQTTClient_message* message)
{
    (void)topicLen;
    auto* self = static_cast<MqttSubscriber*>(context);
    QByteArray payload(static_cast<const char*>(message->payload), message->payloadlen);

    QMetaObject::invokeMethod(
        self,
        [self, payload]() { emit self->messageReceived(payload); },
        Qt::QueuedConnection);

    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

bool MqttSubscriber::connectToBroker(const QString& brokerUri, const QString& clientId)
{
    disconnectFromBroker();

    MQTTClient client = nullptr;
    const int rcCreate = MQTTClient_create(&client,
                                           brokerUri.toUtf8().constData(),
                                           clientId.toUtf8().constData(),
                                           MQTTCLIENT_PERSISTENCE_NONE,
                                           nullptr);
    if (rcCreate != MQTTCLIENT_SUCCESS) {
        emit statusMessage(QStringLiteral("Failed to create MQTT client (%1)").arg(rcCreate));
        return false;
    }

    client_ = client;
    MQTTClient_setCallbacks(client_, this, &MqttSubscriber::onConnectionLost,
                            &MqttSubscriber::onMessageArrived, nullptr);

    MQTTClient_connectOptions opts = MQTTClient_connectOptions_initializer;
    opts.keepAliveInterval = 20;
    opts.cleansession = 1;

    emit statusMessage(QStringLiteral("Connecting to %1 ...").arg(brokerUri));
    const int rc = MQTTClient_connect(client_, &opts);
    if (rc != MQTTCLIENT_SUCCESS) {
        emit statusMessage(QStringLiteral("MQTT connect failed (%1)").arg(rc));
        MQTTClient_destroy(&client_);
        client_ = nullptr;
        connected_ = false;
        emit connectedChanged(false);
        return false;
    }

    connected_ = true;
    emit connectedChanged(true);
    emit statusMessage(QStringLiteral("Connected"));
    return true;
}

bool MqttSubscriber::subscribe(const QString& topic)
{
    if (!client_ || !connected_) {
        return false;
    }

    const int rc = MQTTClient_subscribe(client_, topic.toUtf8().constData(), 1);
    if (rc != MQTTCLIENT_SUCCESS) {
        emit statusMessage(QStringLiteral("Subscribe failed (%1)").arg(rc));
        return false;
    }

    emit statusMessage(QStringLiteral("Subscribed to %1").arg(topic));
    return true;
}

void MqttSubscriber::disconnectFromBroker()
{
    if (!client_) {
        return;
    }

    if (connected_) {
        MQTTClient_disconnect(client_, 1000);
    }
    MQTTClient_destroy(&client_);
    client_ = nullptr;
    connected_ = false;
    emit connectedChanged(false);
}

}  // namespace agri::mqtt
