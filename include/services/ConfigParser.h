#pragma once

#include <Arduino.h>

#include "models/PanelConfig.h"

class ConfigParser {
 public:
  
  static bool parse(const char* payload, PanelConfig& config, String& error);
};
