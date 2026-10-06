/*
 * SmartDisplay - Mandat 08 (projet Ariane)
 * main.cpp : point d'entrée du prototype.
 *
 * Ce fichier sert uniquement à initialiser les composantes et à
 * coordonner l'application. La logique est placée dans des classes
 * dédiées, ajoutées par les prochaines issues :
 *   - Message        (texte + type NORMAL / ALERT)
 *   - IMessageSource (interface pour toute source de message)
 *   - ButtonSource   (bouton local qui simule la réception de messages)
 *   - Display        (écran OLED + icônes)
 *   - LedActuator    (DEL d'alerte)
 *   - StateMachine   (IDLE / MESSAGE_RECEIVED / DISPLAYING / ERROR)
 */

#include <Arduino.h>

void setup() {
  Serial.begin(115200);

  unsigned long now = millis();

  while (!Serial && millis() - now < 2000) {
  }

  Serial.println("SmartDisplay - PlatformIO project initialized");
}

void loop() {
}