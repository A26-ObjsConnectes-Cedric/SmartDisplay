#include "core/StateMachine.h"

#include "config/Config.h"

StateMachine::StateMachine(IMessageSource* source, Display* display, LedActuator* led)
    : currentState(State::IDLE),
      currentMessage(),
      pendingConfig(),
      source(source),
      configSource(nullptr),
      display(display),
      led(led),
      publisher(nullptr),
      stateEnteredAt(0),
      displayStartedAt(0),
      displayDurationMs(Config::DISPLAY_DURATION_MS),
      brightnessPct(Config::DEFAULT_BRIGHTNESS_PCT) {}

void StateMachine::begin() {
  currentState = State::IDLE;
  stateEnteredAt = millis();
  led->turnOff();
  display->showIdle();
}

void StateMachine::update() {
  switch (currentState) {
    case State::IDLE:
      handleIdle();
      break;
    case State::MESSAGE_RECEIVED:
      handleMessageReceived();
      break;
    case State::DISPLAYING:
      handleDisplaying();
      break;
    case State::ACK_SENT:
      handleAckSent();
      break;
    case State::CONFIGURING:
      handleConfiguring();
      break;
    case State::ERROR:
      handleError();
      break;
  }
}

State StateMachine::getState() const { return currentState; }

void StateMachine::setPublisher(IEventPublisher* newPublisher) { publisher = newPublisher; }

void StateMachine::setConfigSource(IConfigSource* newConfigSource) {
  configSource = newConfigSource;
}

unsigned long StateMachine::getDisplayDurationMs() const { return displayDurationMs; }

uint8_t StateMachine::getBrightnessPct() const { return brightnessPct; }

void StateMachine::publishState() {
  if (publisher != nullptr) {
    publisher->publishState(stateName(currentState), led->isOn());
  }
}

void StateMachine::handleIdle() {
  if (source->readMessage(currentMessage)) {
    changeState(State::MESSAGE_RECEIVED);
    return;
  }

  if (configSource != nullptr && configSource->readConfig(pendingConfig)) {
    changeState(State::CONFIGURING);
  }
}

void StateMachine::handleMessageReceived() {
  if (currentMessage.isValid()) {
    display->showMessage(currentMessage);
    displayStartedAt = millis();

    if (currentMessage.getType() == MessageType::ALERT) {
      led->turnOn();
    } else {
      led->turnOff();
    }

    if (publisher != nullptr) {
      publisher->publishMessage(currentMessage);
    }
    changeState(State::DISPLAYING);
  } else {
    led->turnOff();
    display->showError("Invalid message");
    changeState(State::ERROR);
  }
}

void StateMachine::handleDisplaying() {
  if (publisher != nullptr) {
    publisher->publishAck(currentMessage);
  }
  changeState(State::ACK_SENT);
}


void StateMachine::handleAckSent() {
  if (source->readMessage(currentMessage)) {
    changeState(State::MESSAGE_RECEIVED);
    return;
  }

  if (millis() - displayStartedAt >= displayDurationMs) {
    led->turnOff();
    display->showIdle();
    changeState(State::IDLE);
  }
}

void StateMachine::handleConfiguring() {
  if (!pendingConfig.valid) {
    display->showError("Invalid config");
    changeState(State::ERROR);
    return;
  }

  if (pendingConfig.hasDuration) {
    displayDurationMs = static_cast<unsigned long>(pendingConfig.displayDurationS) * 1000UL;
  }
  if (pendingConfig.hasBrightness) {
    brightnessPct = static_cast<uint8_t>(pendingConfig.brightnessPct);
    display->setBrightness(brightnessPct);
  }

  if (publisher != nullptr) {
    publisher->publishConfigAck(static_cast<unsigned int>(displayDurationMs / 1000UL),
                                brightnessPct);
  }
  changeState(State::IDLE);
}

void StateMachine::handleError() {
  if (millis() - stateEnteredAt >= Config::ERROR_DURATION_MS) {
    display->showIdle();
    changeState(State::IDLE);
  }
}

void StateMachine::changeState(State newState) {
  Serial.print("[StateMachine] ");
  Serial.print(stateName(currentState));
  Serial.print(" -> ");
  Serial.println(stateName(newState));

  currentState = newState;
  stateEnteredAt = millis();
  publishState();
}

const char* StateMachine::stateName(State state) {
  switch (state) {
    case State::IDLE:
      return "IDLE";
    case State::MESSAGE_RECEIVED:
      return "MESSAGE_RECEIVED";
    case State::DISPLAYING:
      return "DISPLAYING";
    case State::ACK_SENT:
      return "ACK_SENT";
    case State::CONFIGURING:
      return "CONFIGURING";
    case State::ERROR:
      return "ERROR";
  }
  return "UNKNOWN";
}
