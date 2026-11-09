#pragma once

#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>

#include <functional>

// Service de communication MQTT.
// Responsabilité : maintenir la connexion au broker (reconnexion automatique),
// abonner les topics demandés et transmettre les messages reçus à leur destinataire.
class MqttService {
 public:
  // Fonction appelée avec le contenu (texte) d'un message reçu
  typedef std::function<void(const char* payload)> MessageHandler;

  MqttService(const char* host, uint16_t port, const char* clientId,
              const char* user, const char* password);

  // Configure le client (ne se connecte pas encore : voir update())
  void begin();

  // À appeler à chaque tour de loop() : connecte au broker quand le Wi-Fi est
  // disponible, réessaie toutes les Config::MQTT_RECONNECT_INTERVAL_MS et
  // traite les messages reçus
  void update();

  bool isConnected();

  // Associe un topic à une fonction. L'abonnement réel se fait à chaque (re)connexion.
  // Retourne false si la liste est pleine.
  bool subscribe(const char* topic, MessageHandler handler);

 private:
  static constexpr int MAX_SUBSCRIPTIONS = 4;

  // Taille du tampon de copie d'un message. Un message plus long est tronqué,
  // et il reste invalide pour l'application (plus de 60 caractères).
  static constexpr unsigned int PAYLOAD_BUFFER_SIZE = 128;

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

  WiFiClient wifiClient;
  PubSubClient client;  // doit être déclaré après wifiClient (il l'utilise)

  Subscription subscriptions[MAX_SUBSCRIPTIONS];
  int subscriptionCount;

  bool wasConnected;
  unsigned long lastAttemptMs;
};
