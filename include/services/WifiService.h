#pragma once

#include <Arduino.h>

class WifiService {
private:
  void startConnection();

  const char* ssid;
  const char* password;
  bool wasConnected;          
  unsigned long lastAttemptMs;  

public:
  WifiService(const char* ssid, const char* password);

  void begin();
  void update();
  bool isConnected() const;

  String localIp() const;
};
