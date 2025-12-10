#include "ApiClient.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

bool ApiClient::fetchConfig(const char *deviceId, DeviceRuntimeConfig &cfg)
{
  if (WiFi.status() != WL_CONNECTED)
    return false;

  HTTPClient http;
  String url = String(BACKEND_BASE_URL) + deviceId + "/";
  http.begin(url);
  int code = http.GET();

  if (code == 200)
  {
    DynamicJsonDocument doc(512);
    deserializeJson(doc, http.getString());

    // JSON de ejemplo esperado desde Django:
    // {
    //   "device_id": "...",
    //   "age": 23,
    //   "threshold_deg": 15.0,
    //   "offset_x": 1.2,
    //   "offset_y": -0.5
    // }

    cfg.age = doc["age"] | DEFAULT_AGE;
    cfg.thresholdDeg = doc["threshold_deg"] | DEFAULT_THRESHOLD_DEG;
    cfg.offsetX = doc["offset_x"] | 0.0;
    cfg.offsetY = doc["offset_y"] | 0.0;

    http.end();
    return true;
  }

  http.end();
  return false;
}

bool ApiClient::sendTelemetry(const char *deviceId,
                              const PostureStatus &status,
                              float rawX, float rawY)
{
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("[API] ❌ WiFi no conectado");
    return false;
  }

  HTTPClient http;
  String url = String(BACKEND_BASE_URL) + TELEMETRY_ENDPOINT;

  Serial.print("[API] Enviando SenML a: ");
  Serial.println(url);

  http.begin(url);
  http.addHeader("Content-Type", "application/json");

  /*
   * ===========================
   *   FORMATO SENML ENVIADO
   * ===========================
   * [
   *   {
   *     "bn": "esp32-posture-001",
   *     "bt": 1733,
   *     "bver": 1,
   *     "e": [
   *        { "n": "posture/tilt", "u": "deg", "v": 18.5 },
   *        { "n": "posture/x",    "u": "deg", "v": 10.2 },
   *        { "n": "posture/y",    "u": "deg", "v": 15.7 },
   *        { "n": "posture/bad_posture", "u": "bool", "v": 1 },
   *        { "n": "posture/threshold", "u": "deg", "v": 15.0 }
   *     ]
   *   }
   * ]
   */

  DynamicJsonDocument doc(1024);

  // SenML es un array
  JsonArray root = doc.to<JsonArray>();

  // Pack SenML
  JsonObject pack = root.createNestedObject();
  pack["bn"] = deviceId;
  pack["bt"] = (uint64_t)(millis() / 1000); // tiempo base en segundos
  pack["bver"] = 1;

  // Entradas
  JsonArray e = pack.createNestedArray("e");

  // ---- Inclinación total (magnitud vectorial) ----
  {
    JsonObject m = e.createNestedObject();
    m["n"] = "posture/tilt";
    m["u"] = "deg";
    m["v"] = status.maxAngle; // √(x² + y²)
  }

  // ---- Componentes X y Y ----
  {
    JsonObject m = e.createNestedObject();
    m["n"] = "posture/x";
    m["u"] = "deg";
    m["v"] = status.angleX;
  }

  {
    JsonObject m = e.createNestedObject();
    m["n"] = "posture/y";
    m["u"] = "deg";
    m["v"] = status.angleY;
  }

  // ---- Estado de postura ----
  {
    JsonObject m = e.createNestedObject();
    m["n"] = "posture/bad_posture";
    m["u"] = "bool";
    m["v"] = status.isBadPosture ? 1 : 0;
  }

  // ---- Umbral actual usado ----
  {
    JsonObject m = e.createNestedObject();
    m["n"] = "posture/threshold";
    m["u"] = "deg";
    m["v"] = status.threshold;
  }

  // Serializar JSON
  String body;
  serializeJson(doc, body);

  Serial.println("[API] Payload SenML:");
  Serial.println(body);

  // Enviar
  int code = http.POST(body);

  Serial.print("[API] Código HTTP recibido: ");
  Serial.println(code);

  if (code == 200 || code == 201)
  {
    Serial.println("[API] ✅ Telemetría enviada correctamente.");
    http.end();
    return true;
  }
  else
  {
    Serial.println("[API] ❌ Error enviando telemetría.");
    Serial.println(http.getString()); // respuesta del backend para debug
  }

  http.end();
  return false;
}

void ApiClient::pullUpdatesIfNeeded(const char *deviceId, PostureEvaluator &eval)
{
  if (millis() - _lastConfigCheck < CONFIG_INTERVAL_MS)
    return;
  _lastConfigCheck = millis();

  DeviceRuntimeConfig cfg;
  if (fetchConfig(deviceId, cfg))
  {
    eval.setAge(cfg.age);
    eval.setOffsets(cfg.offsetX, cfg.offsetY);
    eval.setThreshold(cfg.thresholdDeg); // si quieres respetar el umbral exacto del backend
  }
}