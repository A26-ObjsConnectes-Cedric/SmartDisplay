#pragma once

#include "models/Message.h"

class IMessageSource {
 public:
  virtual ~IMessageSource() {}

  virtual void begin() = 0;

  virtual bool readMessage(Message& message) = 0;
};
