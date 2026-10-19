#pragma once

#include <Adafruit_SSD1306.h>

#include "models/Message.h"

class Display {
 public:
  Display();

  bool begin();

  void showIdle();
  void showMessage(const Message& message);
  void showError(const String& reason);

 private:
  void drawHeader(const unsigned char* icon, const char* title);

  void drawBody(const String& text);

  Adafruit_SSD1306 screen;
  bool ready;  
};
