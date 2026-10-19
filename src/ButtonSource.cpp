#include "inputs/ButtonSource.h"

#include "config/Config.h"

namespace {

struct TestMessage {
  const char* text;
  MessageType type;
};

const TestMessage TEST_MESSAGES[] = {
    {"Meeting at 2 PM - Room B", MessageType::NORMAL},
    {"ALERT: evacuation required", MessageType::ALERT},
    {"Coffee is ready", MessageType::NORMAL},
    {"ALERT: network failure", MessageType::ALERT},
    // Message volontairement invalide (vide) : sert à tester l'état ERROR
    {"", MessageType::NORMAL},
};

const int TEST_MESSAGE_COUNT = sizeof(TEST_MESSAGES) / sizeof(TEST_MESSAGES[0]);

}  // namespace

ButtonSource::ButtonSource(uint8_t buttonPin)
    : buttonPin(buttonPin),
      messageIndex(0),
      idleLevel(LOW),
      lastReading(LOW),
      stableLevel(LOW),
      lastChangeTime(0) {}

void ButtonSource::begin() {
  pinMode(buttonPin, INPUT);

  idleLevel = digitalRead(buttonPin);
  lastReading = idleLevel;
  stableLevel = idleLevel;
  lastChangeTime = millis();

  Serial.print("[ButtonSource] idle level: ");
  Serial.println(idleLevel == HIGH ? "HIGH" : "LOW");
}

bool ButtonSource::wasPressed() {
  int reading = digitalRead(buttonPin);
  unsigned long now = millis();

  if (reading != lastReading) {
    lastReading = reading;
    lastChangeTime = now;
  }

  if ((now - lastChangeTime) >= Config::BUTTON_DEBOUNCE_MS && reading != stableLevel) {
    stableLevel = reading;
    return stableLevel != idleLevel;
  }
  return false;
}

bool ButtonSource::readMessage(Message& message) {
  if (!wasPressed()) {
    return false;
  }

  const TestMessage& test = TEST_MESSAGES[messageIndex];
  message = Message(test.text, test.type);
  messageIndex = (messageIndex + 1) % TEST_MESSAGE_COUNT;
  return true;
}
