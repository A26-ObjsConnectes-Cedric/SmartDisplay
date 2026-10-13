#pragma once

#include <Arduino.h>

class LedActuator {
 public:
  explicit LedActuator(uint8_t ledPin);

  void begin();

  void turnOn();
  void turnOff();

  bool isOn() const;

 private:
  void apply(bool on);

  uint8_t ledPin;
  bool ledStatus;
};
