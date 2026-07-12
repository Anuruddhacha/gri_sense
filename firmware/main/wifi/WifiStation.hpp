#pragma once

namespace agri::wifi {

class WifiStation {
public:
    bool connect(const char* ssid, const char* password);
};

}  // namespace agri::wifi
