#pragma once

#include "models/Message.h"

class IEventPublisher {
 public:
  virtual ~IEventPublisher() {}

  virtual void publishState(const char* stateName, bool ledOn) = 0;

  virtual void publishMessage(const Message& message) = 0;
};
