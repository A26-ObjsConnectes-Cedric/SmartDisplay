#include "outputs/LedActuator.h"
#include "config/Config.h"

LedActuator::LedActuator(uint8_t ledPin) : ledPin(ledPin), ledStatus(false) {}

void LedActuator::begin() 
{
  pinMode(ledPin, OUTPUT);
  turnOff();
}

void LedActuator::turnOn() 
{ 
    apply(true); 
}

void LedActuator::turnOff() 
{ 
    apply(false); 
}

bool LedActuator::isOn() const 
{ 
    return ledStatus; 
}

void LedActuator::apply(bool on) 
{
  ledStatus = on;
  bool level = (on == Config::LED_ACTIVE_HIGH);
  digitalWrite(ledPin, level ? HIGH : LOW);
}
