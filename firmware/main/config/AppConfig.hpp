#pragma once

#include <cstdint>

// Edit Wi-Fi / MQTT / pins before flashing
namespace agri::config {

inline constexpr const char* kWifiSsid       = "SLT-4G_597084";
inline constexpr const char* kWifiPassword   = "A6FC8BA5";
inline constexpr const char* kMqttBrokerUri  = "mqtt://broker.hivemq.com";
inline constexpr const char* kMqttTopic      = "agri_sense/telemetry";
inline constexpr const char* kMqttCommandTopic = "agri_sense/command";
inline constexpr const char* kDeviceId       = "esp32-agri-01";
inline constexpr int kPublishIntervalMs      = 5000;

// true  = return demo values from each sensor driver
// false = read real hardware (implement readHardware() in each driver)
inline constexpr bool kSimulateSensors = true;

// true = log pump state only; false = drive relay GPIO
inline constexpr bool kSimulatePump = true;

// Default agri-board pin map
inline constexpr int kDht22Gpio              = 4;
inline constexpr int kSoilMoistureAdcGpio    = 34;
inline constexpr int kSoilPhAdcGpio          = 35;
inline constexpr int kBh1750SdaGpio          = 21;
inline constexpr int kBh1750SclGpio          = 22;
inline constexpr uint8_t kBh1750I2cAddr      = 0x23;
inline constexpr int kWaterPumpGpio          = 26;  // relay IN

// Auto-irrigation thresholds
inline constexpr float kMoistureOnPct   = 35.0f;   // turn pump ON below this
inline constexpr float kMoistureOffPct  = 55.0f;   // turn pump OFF above this
inline constexpr float kTempMinC        = 10.0f;
inline constexpr float kTempMaxC        = 40.0f;
inline constexpr float kHumidityMaxPct  = 90.0f;   // skip watering if air is already very wet
inline constexpr float kLightMinLux     = 50.0f;   // avoid night watering by default

// Start with auto irrigation enabled (dashboard can toggle via MQTT)
inline constexpr bool kPumpEnabledDefault = true;

}  // namespace agri::config
