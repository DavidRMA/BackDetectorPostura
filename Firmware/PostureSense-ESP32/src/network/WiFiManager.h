#pragma once
#include <Arduino.h>

class WiFiManager {
public:
  void begin(const char* ssid, const char* pass);
  bool isConnected();
};