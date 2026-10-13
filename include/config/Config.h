#pragma once

#include <Arduino.h>

namespace Config {

#define SERIAL_BAUD_RATE 115200

constexpr uint8_t BUTTON_PIN = 35;

constexpr uint8_t LED_PIN = 26;

constexpr bool LED_ACTIVE_HIGH = true;

} 
