#pragma once

#include "interfaces/IMessageSource.h"

class ButtonSource : public IMessageSource {
 public:
  explicit ButtonSource(uint8_t buttonPin);

  void begin() override;

  bool readMessage(Message& message) override;

 private:
  bool wasPressed();

  uint8_t buttonPin;
  int messageIndex;
  int idleLevel;                
  int lastReading;            
  int stableLevel;              
  unsigned long lastChangeTime; 
};
