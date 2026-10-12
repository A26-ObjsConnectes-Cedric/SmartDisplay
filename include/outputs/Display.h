#pragma once

#include "models/Message.h"


class Display {
 public:
  Display();

  bool begin();

  void showIdle();
  void showMessage(const Message& message);
  void showError(const String& reason);
};
