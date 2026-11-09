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
      wifiClient(),
      client(wifiClient),
      subscriptionCount(0),
      wasConnected(false),
      lastAttemptMs(0) {}

void MqttService::begin() {
  client.setServer(host, port);
  // Tampon un peu plus grand que celui par défaut (256 octets)
  client.setBufferSize(512);
  // Limite l'attente d'une réponse du broker (secondes)
  client.setSocketTimeout(2);
  client.setCallback([this](char* topic, uint8_t* payload, unsigned int length) {
    onMessage(topic, payload, length);
  });

  // La première tentative peut avoir lieu dès que le Wi-Fi est prêt
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

void MqttService::update() {
  bool connected = client.connected();

  // On n'écrit dans le moniteur série que lors d'un changement d'état
  if (!connected && wasConnected) {
    Serial.println("[Mqtt] connection lost");
    wasConnected = false;
  }

  if (!connected) {
    // Pas de tentative sans Wi-Fi, et seulement toutes les MQTT_RECONNECT_INTERVAL_MS
    if (WiFi.status() == WL_CONNECTED &&
        millis() - lastAttemptMs >= MQTT_RECONNECT_INTERVAL_MS) {
      lastAttemptMs = millis();
      connectToBroker();
    }
    return;
  }

  // Lit les messages reçus (rappelle onMessage)
  client.loop();
}

void MqttService::connectToBroker() {
  Serial.print("[Mqtt] connecting to ");
  Serial.println(host);

  if (!client.connect(clientId, user, password)) {
    Serial.print("[Mqtt] connection failed, rc=");
    Serial.println(client.state());
    return;
  }

  Serial.println("[Mqtt] connected");
  wasConnected = true;

  // Les abonnements sont perdus à chaque déconnexion : on les refait ici
  for (int i = 0; i < subscriptionCount; i++) {
    client.subscribe(subscriptions[i].topic);
    Serial.print("[Mqtt] subscribed to ");
    Serial.println(subscriptions[i].topic);
  }
}

void MqttService::onMessage(char* topic, uint8_t* payload, unsigned int length) {
  // Le contenu reçu n'est pas terminé par un caractère nul : on en fait une copie
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
