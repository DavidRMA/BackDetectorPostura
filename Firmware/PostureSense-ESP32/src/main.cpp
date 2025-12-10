#include <Arduino.h>
#include "config/DeviceConfig.h"
#include "network/WiFiManager.h"
#include "network/ApiClient.h"
#include "sensors/MPU6050Sensor.h"
#include "posture/PostureEvaluator.h"
#include "actuators/Vibrator.h"
#include "posture/Calibration.h"

WiFiManager wifiManager;
ApiClient apiClient;
MPU6050Sensor mpu;
PostureEvaluator postureEval;
Vibrator vibrator(VIBRATOR_PIN);
Calibration calibrator(mpu, postureEval);

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

  // 3. Intentar cargar calibración previa DESDE Calibration
  bool hasCalib = calibrator.loadOffsetsFromEEPROM();

  if (!hasCalib)
  {
    calibrator.runCalibration();
    calibrator.saveOffsetsToEEPROM();
  }

  // 4. (Opcional) obtener config del backend (edad, umbral, etc.)
  DeviceRuntimeConfig cfg;
  if (apiClient.fetchConfig(DEVICE_ID, cfg))
  {
    postureEval.setAge(cfg.age);
    if (cfg.thresholdDeg > 0)
    {
      postureEval.setThreshold(cfg.thresholdDeg);
    }
  }
  else
  {
    postureEval.setAge(DEFAULT_AGE);
  }

  Serial.println("Setup completo, entrando a loop...");
}

void loop()
{
  // 1. Leer sensor
  float angleX, angleY;
  if (!mpu.readAngles(angleX, angleY))
  {
    // si falla lectura, salimos del loop
    return;
  }

  // 2. Evaluar postura (aplica offsets + umbral por edad)
  PostureStatus status = postureEval.evaluate(angleX, angleY);

  // 3. Control del vibrador según estado
  static unsigned long badPostureSince = 0;
  if (status.isBadPosture)
  {
    if (badPostureSince == 0)
      badPostureSince = millis();

    if (millis() - badPostureSince > VIBRATION_DELAY_MS)
    {
      vibrator.patternAlert(); // patrón ya definido
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

  // 5. (Opcional) consultar al backend si hay nueva config
  apiClient.pullUpdatesIfNeeded(DEVICE_ID, postureEval);

  delay(50); // pequeño descanso
}
