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
  stateMachine.begin();
}

void loop() {
  stateMachine.update();

  // TEMPORAIRE: clignotement de test, non bloquant,
  static unsigned long lastToggle = 0;
  if (millis() - lastToggle >= 500) {
    lastToggle = millis();
    if (led.isOn()) {
      led.turnOff();
    } 
    else {
      led.turnOn();
    }
    Serial.println(led.isOn() ? "[test] LED ON" : "[test] LED OFF");
  }
}
