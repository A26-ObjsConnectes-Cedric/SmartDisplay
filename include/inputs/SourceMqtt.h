#pragma once

#include "interfaces/IMessageSource.h"
#include "services/MqttService.h"

// Source de messages MQTT : remplace ButtonSource sans modifier la machine à états.
// Responsabilité : transformer un message reçu sur le topic des commandes en Message.
//
// Format temporaire (texte brut) : le contenu reçu est le texte à afficher.
// S'il commence par « ALERT: », le message est de type ALERT. Le format JSON
// arrive avec la fonctionnalité suivante (validation JSON).
class SourceMqtt : public IMessageSource {
 public:
  SourceMqtt(MqttService& mqtt, const char* commandTopic);

  // Abonne la source au topic des commandes
  void begin() override;

  // Lecture non bloquante : retourne le dernier message reçu, une seule fois
  bool readMessage(Message& message) override;

 private:
  // Appelée par MqttService à chaque message reçu sur le topic des commandes
  void onPayload(const char* payload);

  MqttService& mqtt;
  const char* commandTopic;
  bool hasPending;       // un message est arrivé et n'a pas encore été lu
  Message pendingMessage;
};
