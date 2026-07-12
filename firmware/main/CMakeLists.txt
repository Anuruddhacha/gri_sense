idf_component_register(
    SRCS
        "main.cpp"
        "wifi/WifiStation.cpp"
        "mqtt/MqttPublisher.cpp"
        "telemetry/TelemetryJson.cpp"
        "sensors/SensorBoard.cpp"
        "sensors/Dht22TempHumidity.cpp"
        "sensors/CapacitiveSoilMoisture.cpp"
        "sensors/Bh1750LightSensor.cpp"
        "sensors/AnalogPhSensor.cpp"
        "actuators/WaterPump.cpp"
        "irrigation/IrrigationController.cpp"
    INCLUDE_DIRS "."
    REQUIRES mqtt nvs_flash esp_wifi esp_event esp_netif esp_timer esp_driver_gpio
)
