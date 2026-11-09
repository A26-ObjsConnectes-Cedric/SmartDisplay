#include "inputs/SourceMqtt.h"

#include <string.h>

namespace {
const char* const ALERT_PREFIX = "ALERT:";
}  // namespace

SourceMqtt::SourceMqtt(MqttService& mqtt, const char* commandTopic)
    : mqtt(mqtt), commandTopic(commandTopic), hasPending(false), pendingMessage() {}

void SourceMqtt::begin() {
  mqtt.subscribe(commandTopic, [this](const char* payload) { onPayload(payload); });
}

void SourceMqtt::onPayload(const char* payload) {
  String text(payload);
  text.trim();

  MessageType type = MessageType::NORMAL;
  if (strncmp(text.c_str(), ALERT_PREFIX, strlen(ALERT_PREFIX)) == 0) {
    type = MessageType::ALERT;
  }

  // Un seul message à la fois : s'il y en a déjà un non lu, le plus récent le remplace.
  pendingMessage = Message(text, type);
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
