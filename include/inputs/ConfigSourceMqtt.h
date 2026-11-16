#pragma once

#include "interfaces/IConfigSource.h"
#include "services/MqttService.h"

class ConfigSourceMqtt : public IConfigSource {
 public:
  ConfigSourceMqtt(MqttService& mqtt, const char* configTopic);

  void begin() override;

  bool readConfig(PanelConfig& config) override;

 private:
  void onPayload(const char* payload);

  MqttService& mqtt;
  const char* configTopic;
  bool hasPending;
  PanelConfig pendingConfig;
};
