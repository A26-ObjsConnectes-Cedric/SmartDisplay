#pragma once

#include <Arduino.h>

class LedActuator {
 public:
  explicit LedActuator(uint8_t ledPin);

  void begin();
  void turnOn();
  void turnOff();

 private:
  uint8_t ledPin;
};
