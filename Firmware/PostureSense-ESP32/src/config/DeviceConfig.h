#pragma once
#include <Arduino.h>

#define VIBRATOR_PIN 4

// WiFi
const char* WIFI_SSID = "TU_SSID";
const char* WIFI_PASS = "TU_PASS";

// Backend (Servidor IoT)
const char* BACKEND_BASE_URL = "http://192.168.1.100:8000";  // o dominio
const char* CONFIG_ENDPOINT   = "/api/iot/device-config/";
const char* TELEMETRY_ENDPOINT = "/api/iot/telemetry/";

const char* DEVICE_ID = "esp32-posture-001";

const int DEFAULT_AGE = 25;
const float DEFAULT_THRESHOLD_DEG = 15.0;
const unsigned long VIBRATION_DELAY_MS = 3000;

struct DeviceRuntimeConfig {
  int age;
  float thresholdDeg;
  float offsetX;
  float offsetY;
};
