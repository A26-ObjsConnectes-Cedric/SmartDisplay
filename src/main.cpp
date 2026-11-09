/*
 * SmartDisplay - Mandat 08 (projet Ariane)
 * main.cpp : point d'entrée du prototype.
 */

#include <Arduino.h>

#include "inputs/SourceMqtt.h"
#include "config/Config.h"
#include "config/secrets.h"
#include "outputs/Display.h"
#include "outputs/LedActuator.h"
#include "core/StateMachine.h"
#include "services/MqttService.h"
#include "services/WifiService.h"

// Composantes du prototype (les constructeurs n'accèdent pas au matériel)
Display display;
LedActuator led(Config::LED_PIN);
WifiService wifi(Secrets::WIFI_SSID, Secrets::WIFI_PASSWORD);
MqttService mqtt(MQTT_HOST, MQTT_PORT, MQTT_CLIENT_ID,
                 Secrets::MQTT_USER, Secrets::MQTT_PASSWORD);
// La source MQTT remplace le bouton : la machine à états ne change pas
SourceMqtt sourceMqtt(mqtt, MQTT_TOPIC_CMD);
StateMachine stateMachine(&sourceMqtt, &display, &led);

void setup() {
  Serial.begin(SERIAL_BAUD_RATE);

  led.begin();
  if (!display.begin()) {
    Serial.println("Display initialization failed (check I2C wiring and address)");
  }
  stateMachine.begin();
  wifi.begin();
  mqtt.begin();
  sourceMqtt.begin();
}

void loop() {
  wifi.update();
  mqtt.update();
  stateMachine.update();
}
