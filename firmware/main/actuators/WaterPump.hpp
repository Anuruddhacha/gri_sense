#pragma once

namespace agri::actuators {

// Relay-driven water pump / solenoid valve
class WaterPump {
public:
    explicit WaterPump(int gpio_num);

    bool init();
    void setRunning(bool running);
    bool isRunning() const { return running_; }

private:
    int gpio_num_;
    bool running_ = false;
    bool ready_ = false;
};

}  // namespace agri::actuators
