#include <Arduino.h>
#include "config/DeviceConfig.h"
#include "network/WiFiManager.h"
#include "network/ApiClient.h"
#include "sensors/MPU6050Sensor.h"
#include "posture/PostureEvaluator.h"
#include "actuators/Vibrator.h"

WiFiManager wifiManager;
ApiClient apiClient;
MPU6050Sensor mpu;
PostureEvaluator postureEval;
Vibrator vibrator(VIBRATOR_PIN);

unsigned long lastTelemetry = 0;
const unsigned long TELEMETRY_INTERVAL_MS = 1000; // cada 1s

void setup()
{
  Serial.begin(115200);
  delay(2000);

  Serial.println("\n=== POSTURE CORRECT - FIRMWARE REAL ===");

  Serial.print("DEBUG SSID: [");
  Serial.print(WIFI_SSID);
  Serial.println("]");

  pinMode(VIBRATOR_PIN, OUTPUT);
  digitalWrite(VIBRATOR_PIN, LOW);

  // 1. Conectar WiFi
  wifiManager.begin(WIFI_SSID, WIFI_PASS);

  // 2. Inicializar sensor
  if (!mpu.begin())
  {
    Serial.println("ERROR: MPU6050 no detectado");
  }

  // 3. Obtener configuración del backend (edad -> umbral, offsets, etc.)
  DeviceRuntimeConfig cfg;
  if (apiClient.fetchConfig(DEVICE_ID, cfg))
  {
    // Config trae: edad_usuario, threshold_deg, offsetX, offsetY
    postureEval.setAge(cfg.age);
    postureEval.setThreshold(cfg.thresholdDeg);
    postureEval.setOffsets(cfg.offsetX, cfg.offsetY);
  }
  else
  {
    // fallback: umbral por defecto
    postureEval.setAge(DEFAULT_AGE);
    postureEval.setThreshold(DEFAULT_THRESHOLD_DEG);
  }

  Serial.println("Setup completo, entrando a loop...");
}

void loop()
{
  // 1. Leer sensor
  float angleX, angleY;
  if (!mpu.readAngles(angleX, angleY))
  {
    // si falla lectura, no hacemos nada especial
    return;
  }

  // 2. Evaluar postura (aplica offsets + umbral)
  PostureStatus status = postureEval.evaluate(angleX, angleY);

  // 3. Control del vibrador según estado
  static unsigned long badPostureSince = 0;
  if (status.isBadPosture)
  {
    if (badPostureSince == 0)
      badPostureSince = millis();

    if (millis() - badPostureSince > VIBRATION_DELAY_MS)
    {
      vibrator.patternAlert(); // activa vibración (patrón ya definido en la clase)
    }
  }
  else
  {
    badPostureSince = 0;
    vibrator.off();
  }

  // 4. Enviar telemetría al backend periódicamente
  if (millis() - lastTelemetry > TELEMETRY_INTERVAL_MS)
  {
    apiClient.sendTelemetry(DEVICE_ID, status, angleX, angleY);
    lastTelemetry = millis();
  }

  // 5. (Opcional) preguntar al servidor por nuevas configs
  apiClient.pullUpdatesIfNeeded(DEVICE_ID, postureEval);

  delay(50); // pequeño descanso
}
