#include "inputs/SourceMqtt.h"

#include "services/MessageParser.h"

SourceMqtt::SourceMqtt(MqttService& mqtt, const char* commandTopic)
    : mqtt(mqtt), commandTopic(commandTopic), hasPending(false), pendingMessage() {}

void SourceMqtt::begin() {
  mqtt.subscribe(commandTopic, [this](const char* payload) { onPayload(payload); });
}

void SourceMqtt::onPayload(const char* payload) {
  Message parsed;
  String error;

  if (MessageParser::parse(payload, parsed, error)) {
    pendingMessage = parsed;
  } else {
    Serial.print("[Json] rejected: ");
    Serial.println(error);
    pendingMessage = Message();
  }

  hasPending = true;
}

bool SourceMqtt::readMessage(Message& message) {
  if (!hasPending) {
    return false;
  }
  message = pendingMessage;
  hasPending = false;
  return true;
}
