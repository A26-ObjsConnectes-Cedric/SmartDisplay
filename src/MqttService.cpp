#include "services/MqttService.h"

#include <string.h>

#include "config/Config.h"

MqttService::MqttService(const char* host, uint16_t port, const char* clientId,
                         const char* user, const char* password)
    : host(host),
      port(port),
      clientId(clientId),
      user(user),
      password(password),
      willTopic(nullptr),
      willPayload(nullptr),
      connectHandler(),
      wifiClient(),
      client(wifiClient),
      subscriptionCount(0),
      wasConnected(false),
      lastAttemptMs(0) {}

void MqttService::begin() {
  client.setServer(host, port);
  client.setBufferSize(512);
  client.setSocketTimeout(2);
  client.setCallback([this](char* topic, uint8_t* payload, unsigned int length) {
    onMessage(topic, payload, length);
  });

  lastAttemptMs = millis() - MQTT_RECONNECT_INTERVAL_MS;
}

bool MqttService::subscribe(const char* topic, MessageHandler handler) {
  if (subscriptionCount >= MAX_SUBSCRIPTIONS) {
    return false;
  }
  subscriptions[subscriptionCount].topic = topic;
  subscriptions[subscriptionCount].handler = handler;
  subscriptionCount++;
  return true;
}

bool MqttService::isConnected() { return client.connected(); }

void MqttService::setWill(const char* topic, const char* payload) {
  willTopic = topic;
  willPayload = payload;
}

void MqttService::setConnectHandler(ConnectHandler handler) { connectHandler = handler; }

bool MqttService::publish(const char* topic, const char* payload, bool retained) {
  if (!client.connected()) {
    return false;
  }
  return client.publish(topic, payload, retained);
}

void MqttService::update() {
  bool connected = client.connected();

  if (!connected && wasConnected) {
    Serial.println("[Mqtt] connection lost");
    wasConnected = false;
  }

  if (!connected) {
    if (WiFi.status() == WL_CONNECTED &&
        millis() - lastAttemptMs >= MQTT_RECONNECT_INTERVAL_MS) {
      lastAttemptMs = millis();
      connectToBroker();
    }
    return;
  }

  client.loop();
}

void MqttService::connectToBroker() {
  Serial.print("[Mqtt] connecting to ");
  Serial.println(host);

  bool connected = willTopic != nullptr
                       ? client.connect(clientId, user, password, willTopic, 0, true, willPayload)
                       : client.connect(clientId, user, password);
  if (!connected) {
    Serial.print("[Mqtt] connection failed, rc=");
    Serial.println(client.state());
    return;
  }

  Serial.println("[Mqtt] connected");
  wasConnected = true;

  for (int i = 0; i < subscriptionCount; i++) {
    client.subscribe(subscriptions[i].topic);
    Serial.print("[Mqtt] subscribed to ");
    Serial.println(subscriptions[i].topic);
  }

  if (connectHandler) {
    connectHandler();
  }
}

void MqttService::onMessage(char* topic, uint8_t* payload, unsigned int length) {
  char text[PAYLOAD_BUFFER_SIZE];
  unsigned int copied = length < PAYLOAD_BUFFER_SIZE - 1 ? length : PAYLOAD_BUFFER_SIZE - 1;
  memcpy(text, payload, copied);
  text[copied] = '\0';

  for (int i = 0; i < subscriptionCount; i++) {
    if (strcmp(subscriptions[i].topic, topic) == 0 && subscriptions[i].handler) {
      subscriptions[i].handler(text);
    }
  }
}
