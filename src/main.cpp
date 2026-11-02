/*
 * SmartDisplay - Mandat 08 (projet Ariane)
 * main.cpp : point d'entrée du prototype.
 */

#include <Arduino.h>

#include "inputs/ButtonSource.h"
#include "config/Config.h"
#include "config/secrets.h"
#include "outputs/Display.h"
#include "outputs/LedActuator.h"
#include "core/StateMachine.h"
#include "services/WifiService.h"

ButtonSource buttonSource(Config::BUTTON_PIN);
Display display;
LedActuator led(Config::LED_PIN);
StateMachine stateMachine(&buttonSource, &display, &led);
WifiService wifi(Secrets::WIFI_SSID, Secrets::WIFI_PASSWORD);


void setup() {
  Serial.begin(SERIAL_BAUD_RATE);

  buttonSource.begin();
  led.begin();
  display.begin();
  stateMachine.begin();
}

void loop() {
  wifi.update();
  stateMachine.update();
}
