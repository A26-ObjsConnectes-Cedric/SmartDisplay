#include "services/MessageParser.h"

#include <ArduinoJson.h>
#include <string.h>

bool MessageParser::parse(const char* payload, Message& message, String& error) {
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

 
  if (!doc["id"].is<const char*>()) {
    error = "field 'id' missing or not a string";
    return false;
  }
  if (!doc["text"].is<const char*>()) {
    error = "field 'text' missing or not a string";
    return false;
  }
  if (!doc["type"].is<const char*>()) {
    error = "field 'type' missing or not a string";
    return false;
  }

  const char* id = doc["id"];
  const char* text = doc["text"];
  const char* type = doc["type"];

  size_t idLength = strlen(id);
  if (idLength == 0 || idLength > MessageParser::MAX_ID_LENGTH) {
    error = "field 'id' empty or too long";
    return false;
  }

  size_t textLength = strlen(text);
  if (textLength == 0) {
    error = "field 'text' is empty";
    return false;
  }
  if (textLength > Message::MAX_TEXT_LENGTH) {
    error = "field 'text' too long";
    return false;
  }

  MessageType messageType;
  if (strcmp(type, "info") == 0) {
    messageType = MessageType::NORMAL;
  } else if (strcmp(type, "alert") == 0) {
    messageType = MessageType::ALERT;
  } else {
    error = "field 'type' must be 'info' or 'alert'";
    return false;
  }

  message = Message(String(text), messageType, String(id));
  return true;
}
