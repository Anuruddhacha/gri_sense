#pragma once

#include "core/TelemetryData.hpp"

#include <QByteArray>
#include <optional>

namespace agri::core {

class TelemetryParser {
public:
    static std::optional<TelemetryData> fromJson(const QByteArray& payload);
};

}  // namespace agri::core
