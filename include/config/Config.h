#pragma once

#include <Arduino.h>

// Configuration centrale du prototype : broches et constantes matérielles.
// Les numéros de broches sont à valider selon le câblage réel.
namespace Config {

constexpr uint8_t BUTTON_PIN = 4;

constexpr uint8_t LED_PIN = 2;

constexpr unsigned long SERIAL_BAUD_RATE = 115200;

} 