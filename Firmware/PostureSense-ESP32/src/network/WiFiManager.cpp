#include "WiFiManager.h"
#include <WiFi.h>

void WiFiManager::begin(const char* ssid, const char* pass) {
  WiFi.begin(ssid, pass);
  Serial.print("Conectando a WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado, IP: " + WiFi.localIP().toString());
}

bool WiFiManager::isConnected() {
  return WiFi.status() == WL_CONNECTED;
}