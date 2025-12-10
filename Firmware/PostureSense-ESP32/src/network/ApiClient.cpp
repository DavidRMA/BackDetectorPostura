#include "ApiClient.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

bool ApiClient::fetchConfig(const char* deviceId, DeviceRuntimeConfig& cfg) {
  if (WiFi.status() != WL_CONNECTED) return false;

  HTTPClient http;
  String url = String(BACKEND_BASE_URL) + CONFIG_ENDPOINT + deviceId + "/";
  http.begin(url);
  int code = http.GET();

  if (code == 200) {
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

    cfg.age         = doc["age"] | DEFAULT_AGE;
    cfg.thresholdDeg= doc["threshold_deg"] | DEFAULT_THRESHOLD_DEG;
    cfg.offsetX     = doc["offset_x"] | 0.0;
    cfg.offsetY     = doc["offset_y"] | 0.0;

    http.end();
    return true;
  }

  http.end();
  return false;
}

bool ApiClient::sendTelemetry(const char* deviceId,
                              const PostureStatus& status,
                              float rawX, float rawY) {
  if (WiFi.status() != WL_CONNECTED) return false;

  HTTPClient http;
  String url = String(BACKEND_BASE_URL) + TELEMETRY_ENDPOINT;
  http.begin(url);
  http.addHeader("Content-Type", "application/json");

  // SenML-like: array de un solo pack
  DynamicJsonDocument doc(1024);

  // Raíz = array
  JsonArray root = doc.to<JsonArray>();

  // Pack SenML
  JsonObject pack = root.createNestedObject();
  pack["bn"]   = deviceId;            // base name = id del dispositivo
  pack["bt"]   = (uint64_t)(millis() / 1000); // tiempo base en segundos desde arranque
  pack["bver"] = 1;

  // Array de entradas "e"
  JsonArray e = pack.createNestedArray("e");

  // 1) Inclinación total (magnitud del vector)
  {
    JsonObject m = e.createNestedObject();
    m["n"] = "posture/tilt";
    m["u"] = "deg";
    m["v"] = status.maxAngle; // magnitud del vector ya calculada
  }

  // 2) Componente X
  {
    JsonObject m = e.createNestedObject();
    m["n"] = "posture/x";
    m["u"] = "deg";
    m["v"] = status.angleX;
  }

  // 3) Componente Y
  {
    JsonObject m = e.createNestedObject();
    m["n"] = "posture/y";
    m["u"] = "deg";
    m["v"] = status.angleY;
  }

  // 4) Estado de postura
  {
    JsonObject m = e.createNestedObject();
    m["n"] = "posture/bad_posture";
    m["u"] = "bool";
    m["v"] = status.isBadPosture ? 1 : 0;
  }

  // 5) (Opcional) umbral actual
  {
    JsonObject m = e.createNestedObject();
    m["n"] = "posture/threshold";
    m["u"] = "deg";
    m["v"] = status.threshold;
  }

  String body;
  serializeJson(doc, body);

  int code = http.POST(body);
  http.end();

  return (code == 200 || code == 201);
}

void ApiClient::pullUpdatesIfNeeded(const char* deviceId, PostureEvaluator& eval) {
  if (millis() - _lastConfigCheck < CONFIG_INTERVAL_MS) return;
  _lastConfigCheck = millis();

  DeviceRuntimeConfig cfg;
  if (fetchConfig(deviceId, cfg)) {
    eval.setAge(cfg.age);
    eval.setOffsets(cfg.offsetX, cfg.offsetY);
    eval.setThreshold(cfg.thresholdDeg); // si quieres respetar el umbral exacto del backend
  }
}