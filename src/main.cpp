/*
 * SmartDisplay - Mandat 08 (projet Ariane)
 * main.cpp : point d'entrée du prototype.
 *
 * Ce fichier sert uniquement à créer les objets, à initialiser les
 * composantes et à coordonner l'application. Toute la logique est dans
 * les classes (voir include/).
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
  Serial.begin(Config::SERIAL_BAUD_RATE);

  buttonSource.begin();
  led.begin();
  if (!display.begin()) {
    Serial.println("Display initialization failed");
  }
  stateMachine.begin();
}

void loop() {
  stateMachine.update();
}
