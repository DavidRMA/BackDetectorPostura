#pragma once
#include <Arduino.h>

#define VIBRATOR_PIN 4

// 🔹 WiFi (ahora con macros, NO con const char*)
#define WIFI_SSID        "Chiquiflow"
#define WIFI_PASS        "1234567890"

// 🔹 Backend (Servidor IoT)
#define BACKEND_BASE_URL   "http://10.15.244.58:8000/api"
#define TELEMETRY_ENDPOINT "/registros_postura/leer_registro/?usuario_id=1"

#define DEVICE_ID          "esp32-posture-001"

// Parámetros de postura
const int DEFAULT_AGE = 25;
const float DEFAULT_THRESHOLD_DEG = 15.0;
const unsigned long VIBRATION_DELAY_MS = 3000;

struct DeviceRuntimeConfig {
  int age;
  float thresholdDeg;
  float offsetX;
  float offsetY;
};
