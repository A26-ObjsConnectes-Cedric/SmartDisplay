#include "outputs/MqttPublisher.h"

#include <ArduinoJson.h>

namespace {

const size_t JSON_BUFFER_SIZE = 384;
}  // namespace

MqttPublisher::MqttPublisher(MqttService& mqtt, const char* stateTopic, const char* dataTopic,
                             const char* ackTopic)
    : mqtt(mqtt), stateTopic(stateTopic), dataTopic(dataTopic), ackTopic(ackTopic) {}

void MqttPublisher::publishState(const char* stateName, bool ledOn) {
  JsonDocument doc;
  doc["state"] = stateName;
  doc["led"] = ledOn;
  doc["uptime_s"] = millis() / 1000;

  char buffer[JSON_BUFFER_SIZE];
  serializeJson(doc, buffer, sizeof(buffer));
  
  send(stateTopic, buffer, true);
}

void MqttPublisher::publishMessage(const Message& message) {
  JsonDocument doc;
  doc["id"] = message.getId().c_str();
  doc["text"] = message.getText().c_str();
  doc["type"] = message.getType() == MessageType::ALERT ? "alert" : "info";

  char buffer[JSON_BUFFER_SIZE];
  serializeJson(doc, buffer, sizeof(buffer));
  send(dataTopic, buffer, false);
}

void MqttPublisher::publishAck(const Message& message) {
  JsonDocument doc;
  doc["id"] = message.getId().c_str();
  doc["status"] = "displayed";
  doc["type"] = message.getType() == MessageType::ALERT ? "alert" : "info";

  char buffer[JSON_BUFFER_SIZE];
  serializeJson(doc, buffer, sizeof(buffer));
  send(ackTopic, buffer, false);
}

void MqttPublisher::publishConfigAck(unsigned int displayDurationS, unsigned int brightnessPct) {
  JsonDocument doc;
  doc["status"] = "configured";
  doc["display_duration_s"] = displayDurationS;
  doc["brightness_pct"] = brightnessPct;

  char buffer[JSON_BUFFER_SIZE];
  serializeJson(doc, buffer, sizeof(buffer));
  send(ackTopic, buffer, false);
}

void MqttPublisher::send(const char* topic, const char* payload, bool retained) {
  if (mqtt.publish(topic, payload, retained)) {
    Serial.print("[Mqtt] published ");
    Serial.print(topic);
    Serial.print(" ");
    Serial.println(payload);
  }
}
