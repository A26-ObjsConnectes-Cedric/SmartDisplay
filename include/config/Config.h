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

} 