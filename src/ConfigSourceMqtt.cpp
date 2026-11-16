#include "inputs/ConfigSourceMqtt.h"

#include "services/ConfigParser.h"

ConfigSourceMqtt::ConfigSourceMqtt(MqttService& mqtt, const char* configTopic)
    : mqtt(mqtt), configTopic(configTopic), hasPending(false), pendingConfig() {}

void ConfigSourceMqtt::begin() {
  mqtt.subscribe(configTopic, [this](const char* payload) { onPayload(payload); });
}

void ConfigSourceMqtt::onPayload(const char* payload) {
  PanelConfig parsed;
  String error;

  if (ConfigParser::parse(payload, parsed, error)) {
    pendingConfig = parsed;
  } else {
    Serial.print("[Json] config rejected: ");
    Serial.println(error);
    pendingConfig = PanelConfig();
  }

  hasPending = true;
}

bool ConfigSourceMqtt::readConfig(PanelConfig& config) {
  if (!hasPending) {
    return false;
  }
  config = pendingConfig;
  hasPending = false;
  return true;
}
