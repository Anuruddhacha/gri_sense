#pragma once

#include <cstdint>

// Edit Wi-Fi / MQTT / pins before flashing
namespace agri::config {

inline constexpr const char* kWifiSsid       = "SLT-4G_597084";
inline constexpr const char* kWifiPassword   = "A6FC8BA5";
inline constexpr const char* kMqttBrokerUri  = "mqtt://broker.hivemq.com";
inline constexpr const char* kMqttTopic      = "agri_sense/telemetry";
inline constexpr const char* kDeviceId       = "esp32-agri-01";
inline constexpr int kPublishIntervalMs      = 5000;

// true  = return demo values from each sensor driver
// false = read real hardware (implement readHardware() in each driver)
inline constexpr bool kSimulateSensors = true;

// Default agri-board pin map
inline constexpr int kDht22Gpio              = 4;
inline constexpr int kSoilMoistureAdcGpio    = 34;
inline constexpr int kSoilPhAdcGpio          = 35;
inline constexpr int kBh1750SdaGpio          = 21;
inline constexpr int kBh1750SclGpio          = 22;
inline constexpr uint8_t kBh1750I2cAddr      = 0x23;

}  // namespace agri::config
