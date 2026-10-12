#include "inputs/ButtonSource.h"

ButtonSource::ButtonSource(uint8_t buttonPin)
    : buttonPin(buttonPin), messageIndex(0), lastPressTime(0) {}

void ButtonSource::begin() 
{
    
}

bool ButtonSource::readMessage(Message& message)
{
  (void)message;
  return false;
}
