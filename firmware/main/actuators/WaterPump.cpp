#include "actuators/WaterPump.hpp"

#include "config/AppConfig.hpp"
#include "driver/gpio.h"
#include "esp_log.h"

namespace agri::actuators {

namespace {
const char* TAG = "WaterPump";
}

WaterPump::WaterPump(int gpio_num)
    : gpio_num_(gpio_num)
{
}

bool WaterPump::init()
{
    if (agri::config::kSimulatePump) {
        ESP_LOGI(TAG, "init GPIO%d (simulate=1)", gpio_num_);
        ready_ = true;
        return true;
    }

    gpio_config_t io = {};
    io.pin_bit_mask = 1ULL << gpio_num_;
    io.mode = GPIO_MODE_OUTPUT;
    io.pull_up_en = GPIO_PULLUP_DISABLE;
    io.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io.intr_type = GPIO_INTR_DISABLE;

    const esp_err_t err = gpio_config(&io);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "gpio_config failed: %s", esp_err_to_name(err));
        return false;
    }

    gpio_set_level(static_cast<gpio_num_t>(gpio_num_), 0);
    ready_ = true;
    ESP_LOGI(TAG, "init GPIO%d (simulate=0)", gpio_num_);
    return true;
}

void WaterPump::setRunning(bool running)
{
    if (!ready_ || running_ == running) {
        return;
    }

    running_ = running;
    if (!agri::config::kSimulatePump) {
        gpio_set_level(static_cast<gpio_num_t>(gpio_num_), running ? 1 : 0);
    }
    ESP_LOGI(TAG, "pump %s", running_ ? "RUNNING" : "STOPPED");
}

}  // namespace agri::actuators
