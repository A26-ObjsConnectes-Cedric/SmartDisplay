#pragma once

#include "interfaces/IEventPublisher.h"
#include "services/MqttService.h"


class MqttPublisher : public IEventPublisher {
 public:
  MqttPublisher(MqttService& mqtt, const char* stateTopic, const char* dataTopic);

  void publishState(const char* stateName, bool ledOn) override;
  void publishMessage(const Message& message) override;

 private:
  void send(const char* topic, const char* payload, bool retained);

  MqttService& mqtt;
  const char* stateTopic;
  const char* dataTopic;
};
