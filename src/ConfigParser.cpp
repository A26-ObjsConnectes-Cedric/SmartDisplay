#include "services/ConfigParser.h"

#include <ArduinoJson.h>

#include "config/Config.h"

bool ConfigParser::parse(const char* payload, PanelConfig& config, String& error) {
  JsonDocument doc;
  DeserializationError result = deserializeJson(doc, payload);
  if (result) {
    error = "invalid JSON";
    return false;
  }

  if (!doc.is<JsonObject>()) {
    error = "JSON object expected";
    return false;
  }

  PanelConfig parsed;

  JsonVariantConst duration = doc["display_duration_s"];
  if (!duration.isNull()) {
    if (!duration.is<int>()) {
      error = "field 'display_duration_s' must be an integer";
      return false;
    }
    int value = duration.as<int>();
    if (value < static_cast<int>(Config::MIN_DISPLAY_DURATION_S) ||
        value > static_cast<int>(Config::MAX_DISPLAY_DURATION_S)) {
      error = "field 'display_duration_s' out of range";
      return false;
    }
    parsed.hasDuration = true;
    parsed.displayDurationS = static_cast<unsigned int>(value);
  }

  JsonVariantConst brightness = doc["brightness_pct"];
  if (!brightness.isNull()) {
    if (!brightness.is<int>()) {
      error = "field 'brightness_pct' must be an integer";
      return false;
    }
    int value = brightness.as<int>();
    if (value < 0 || value > static_cast<int>(Config::MAX_BRIGHTNESS_PCT)) {
      error = "field 'brightness_pct' out of range";
      return false;
    }
    parsed.hasBrightness = true;
    parsed.brightnessPct = static_cast<unsigned int>(value);
  }

  if (!parsed.hasDuration && !parsed.hasBrightness) {
    error = "no known field";
    return false;
  }

  parsed.valid = true;
  config = parsed;
  return true;
}
