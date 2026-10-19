/*
 * SmartDisplay - Mandat 08 (projet Ariane)
 * main.cpp : point d'entrée du prototype.
 */

#include <Arduino.h>

#include "inputs/ButtonSource.h"
#include "config/Config.h"
#include "outputs/Display.h"
#include "outputs/LedActuator.h"
#include "core/StateMachine.h"

ButtonSource buttonSource(Config::BUTTON_PIN);
Display display;
LedActuator led(Config::LED_PIN);
StateMachine stateMachine(&buttonSource, &display, &led);

void setup() {
  Serial.begin(SERIAL_BAUD_RATE);

  buttonSource.begin();
  led.begin();
  display.begin();
  stateMachine.begin();
}

void loop() {
  stateMachine.update();

  // TEMPORAIRE (issue feature/oled-display) : test de l'affichage.
  // Chaque appui affiche le message de test sur l'OLED, et la DEL suit le type
  // du message. À SUPPRIMER dans l'issue feature/state-machine, où la machine
  // à états prend en charge ce comportement.
  Message message;
  if (buttonSource.readMessage(message)) {
    if (message.isValid()) {
      display.showMessage(message);
    } else {
      display.showError("Invalid message");
    }
    if (message.getType() == MessageType::ALERT) {
      led.turnOn();
    } else {
      led.turnOff();
    }
  }
}
