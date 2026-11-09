#pragma once

#include <Arduino.h>

#include "models/Message.h"

class MessageParser {
 public:
  static constexpr size_t MAX_ID_LENGTH = 32;


  static bool parse(const char* payload, Message& message, String& error);
};
