#include "outputs/Display.h"

Display::Display() {}

bool Display::begin() 
{
  return true;
}

void Display::showIdle() 
{ 
    Serial.println("[Display] idle"); 
}

void Display::showMessage(const Message& message) 
{
  Serial.print("[Display] message: ");
  Serial.println(message.getText());
}

void Display::showError(const String& reason) {
  Serial.print("[Display] error: ");
  Serial.println(reason);
}
