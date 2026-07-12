#pragma once

#include <QFrame>
#include <QLabel>
#include <QString>

namespace agri::ui {

class SensorTile : public QFrame {
    Q_OBJECT

public:
    SensorTile(const QString& title, const QString& unit, QWidget* parent = nullptr);

    void setValue(double value, int decimals = 1);
    void setWaiting();

private:
    QLabel* titleLabel_ = nullptr;
    QLabel* valueLabel_ = nullptr;
    QLabel* unitLabel_ = nullptr;
    QString unit_;
};

}  // namespace agri::ui
