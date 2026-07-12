#include "model/TelemetryModel.hpp"

#include <QDateTime>

namespace agri::model {

TelemetryModel::TelemetryModel(QObject* parent)
    : QObject(parent)
{
}

void TelemetryModel::update(const core::TelemetryData& data)
{
    data_ = data;
    lastUpdateText_ = QDateTime::currentDateTime().toString(QStringLiteral("hh:mm:ss"));
    emit telemetryChanged();
}

}  // namespace agri::model
