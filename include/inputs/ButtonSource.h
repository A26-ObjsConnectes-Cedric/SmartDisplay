#pragma once

#include "interfaces/IMessageSource.h"

class ButtonSource : public IMessageSource {
 public:
  explicit ButtonSource(uint8_t buttonPin);

  void begin() override;
  bool readMessage(Message& message) override;

 private:
  uint8_t buttonPin;
  int messageIndex;
  unsigned long lastPressTime;
};
