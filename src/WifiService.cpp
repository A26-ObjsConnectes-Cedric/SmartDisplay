#include "services/WifiService.h"

#include <WiFi.h>

#include "config/Config.h"

WifiService::WifiService(const char* ssid, const char* password)
    : ssid(ssid), password(password), wasConnected(false), lastAttemptMs(0) {}

void WifiService::begin() {
  WiFi.mode(WIFI_STA);
  startConnection();
}

void WifiService::startConnection() {
  Serial.print("[Wifi] connecting to ");
  Serial.println(ssid);

  WiFi.disconnect();
  WiFi.begin(ssid, password);
  lastAttemptMs = millis();
}

void WifiService::update() {
  bool connected = isConnected();

  if (connected && !wasConnected) {
    Serial.print("[Wifi] connected, IP ");
    Serial.println(localIp());
  } else if (!connected && wasConnected) {
    Serial.println("[Wifi] connection lost");
  }
  wasConnected = connected;


  if (!connected && millis() - lastAttemptMs >= WIFI_RETRY_INTERVAL_MS) {
    startConnection();
  }
}

bool WifiService::isConnected() const { return WiFi.status() == WL_CONNECTED; }

String WifiService::localIp() const {
  if (!isConnected()) return String("");
  return WiFi.localIP().toString();
}
