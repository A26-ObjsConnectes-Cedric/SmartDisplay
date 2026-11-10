#pragma once

#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>

#include <functional>

class MqttService {
 public:
  typedef std::function<void(const char* payload)> MessageHandler;

  typedef std::function<void()> ConnectHandler;

  MqttService(const char* host, uint16_t port, const char* clientId,
              const char* user, const char* password);

  void begin();

  void update();

  bool isConnected();

  void setWill(const char* topic, const char* payload);

  void setConnectHandler(ConnectHandler handler);

  bool publish(const char* topic, const char* payload, bool retained = false);


  bool subscribe(const char* topic, MessageHandler handler);

 private:
  static constexpr int MAX_SUBSCRIPTIONS = 4;

  static constexpr unsigned int PAYLOAD_BUFFER_SIZE = 256;

  struct Subscription {
    const char* topic;
    MessageHandler handler;
  };

  void connectToBroker();
  void onMessage(char* topic, uint8_t* payload, unsigned int length);

  const char* host;
  uint16_t port;
  const char* clientId;
  const char* user;
  const char* password;
  const char* willTopic;    
  const char* willPayload;
  ConnectHandler connectHandler;

  WiFiClient wifiClient;
  PubSubClient client;  

  Subscription subscriptions[MAX_SUBSCRIPTIONS];
  int subscriptionCount;

  bool wasConnected;
  unsigned long lastAttemptMs;
};
