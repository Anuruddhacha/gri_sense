#include "ui/SensorTile.hpp"

#include <QVBoxLayout>

namespace agri::ui {

SensorTile::SensorTile(const QString& title, const QString& unit, QWidget* parent)
    : QFrame(parent), unit_(unit)
{
    setObjectName(QStringLiteral("sensorTile"));
    setFrameShape(QFrame::NoFrame);

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(20, 18, 20, 18);
    layout->setSpacing(6);

    titleLabel_ = new QLabel(title, this);
    titleLabel_->setObjectName(QStringLiteral("tileTitle"));

    valueLabel_ = new QLabel(QStringLiteral("--"), this);
    valueLabel_->setObjectName(QStringLiteral("tileValue"));

    unitLabel_ = new QLabel(unit, this);
    unitLabel_->setObjectName(QStringLiteral("tileUnit"));

    layout->addWidget(titleLabel_);
    layout->addWidget(valueLabel_);
    layout->addWidget(unitLabel_);
    layout->addStretch();
}

void SensorTile::setValue(double value, int decimals)
{
    valueLabel_->setText(QString::number(value, 'f', decimals));
}

void SensorTile::setWaiting()
{
    valueLabel_->setText(QStringLiteral("--"));
}

}  // namespace agri::ui
