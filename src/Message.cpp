#include "models/Message.h"

Message::Message() : text(""), type(MessageType::NORMAL) {}

Message::Message(const String& text, MessageType type) : text(text), type(type) {}

const String& Message::getText() const { return text; }

MessageType Message::getType() const 
{ 
    return type; 
}

bool Message::isValid() const 
{
  return text.length() > 0 && text.length() <= MAX_TEXT_LENGTH;

}
