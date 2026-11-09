#pragma once

#include "interfaces/IMessageSource.h"
#include "services/MqttService.h"

class SourceMqtt : public IMessageSource {
 public:
  SourceMqtt(MqttService& mqtt, const char* commandTopic);

  void begin() override;

  bool readMessage(Message& message) override;

 private:
  void onPayload(const char* payload);

  MqttService& mqtt;
  const char* commandTopic;
  bool hasPending;       
  Message pendingMessage;
};
