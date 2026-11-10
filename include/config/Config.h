#pragma once

#include <Arduino.h>

namespace Config {

#define SERIAL_BAUD_RATE 115200

constexpr uint8_t BUTTON_PIN = 35;
constexpr uint8_t LED_PIN = 26;
constexpr bool LED_ACTIVE_HIGH = true;

constexpr unsigned long BUTTON_DEBOUNCE_MS = 10;


// Écran OLED SSD1306 128x64 en I2C (broches I2C par défaut de l'ESP32)
constexpr uint8_t OLED_WIDTH = 128;
constexpr uint8_t OLED_HEIGHT = 64;
constexpr uint8_t OLED_I2C_ADDRESS = 0x3C; 
constexpr uint8_t OLED_SDA_PIN = 21;
constexpr uint8_t OLED_SCL_PIN = 22;

constexpr unsigned long DISPLAY_DURATION_MS = 10000;

constexpr unsigned long ERROR_DURATION_MS = 3000;


#define WIFI_RETRY_INTERVAL_MS 5000

#define MQTT_HOST "mqtt.edwrdl-cedric.com"
#define MQTT_PORT 1883
#define MQTT_CLIENT_ID "smartdisplay-esp32"
#define MQTT_RECONNECT_INTERVAL_MS 5000

#define MQTT_TOPIC_CMD "ariane/smartdisplay/cmd"
#define  MQTT_TOPIC_STATE "ariane/smartdisplay/state"
#define MQTT_TOPIC_DATA  "ariane/smartdisplay/data"

constexpr const char* MQTT_WILL_PAYLOAD = "{\"state\":\"OFFLINE\"}";

} 