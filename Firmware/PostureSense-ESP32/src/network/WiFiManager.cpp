#include "WiFiManager.h"
#include <WiFi.h>

void WiFiManager::begin(const char* ssid, const char* pass) {
  Serial.println();
  Serial.print("[WiFi] Conectando a SSID: ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  unsigned long start = millis();
  const unsigned long TIMEOUT_MS = 15000; // 15 segundos

  while (WiFi.status() != WL_CONNECTED && (millis() - start) < TIMEOUT_MS) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("[WiFi] ✅ Conectado. IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("[WiFi] ❌ No se pudo conectar dentro del tiempo límite.");
    Serial.print("[WiFi] Estado: ");
    Serial.println(WiFi.status());  // Te da un número (WL_CONNECT_FAILED, etc.)

    // Opcional: podrías reintentar más tarde desde loop()
  }
}

bool WiFiManager::isConnected() {
  return WiFi.status() == WL_CONNECTED;
}