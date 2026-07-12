#pragma once

namespace agri::config {

inline constexpr const char* kBrokerUri     = "tcp://broker.hivemq.com:1883";
inline constexpr const char* kTopic         = "agri_sense/telemetry";
inline constexpr const char* kCommandTopic  = "agri_sense/command";
inline constexpr const char* kClientId      = "agri_sense_qt_dash";

}  // namespace agri::config
