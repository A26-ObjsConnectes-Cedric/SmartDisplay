#pragma once

#include <Arduino.h>

enum class MessageType
 { 
    NORMAL,
    ALERT 
};

class Message {
 public:

  static constexpr size_t MAX_TEXT_LENGTH = 60;

  Message();
  Message(const String& text, MessageType type);

  const String& getText() const;
  MessageType getType() const;

  bool isValid() const;

 private:
  String text;
  MessageType type;
};
