#include "core/StateMachine.h"

#include "config/Config.h"

StateMachine::StateMachine(IMessageSource* source, Display* display, LedActuator* led)
    : currentState(State::IDLE),
      currentMessage(),
      source(source),
      display(display),
      led(led),
      stateEnteredAt(0) {}

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
    case State::ERROR:
      handleError();
      break;
  }
}

State StateMachine::getState() const { return currentState; }

void StateMachine::handleIdle() {
  if (source->readMessage(currentMessage)) {
    changeState(State::MESSAGE_RECEIVED);
  }
}

void StateMachine::handleMessageReceived() {
  if (currentMessage.isValid()) {
    display->showMessage(currentMessage);
    if (currentMessage.getType() == MessageType::ALERT) {
      led->turnOn();
    } else {
      led->turnOff();
    }
    changeState(State::DISPLAYING);
  } else {
    led->turnOff();
    display->showError("Invalid message");
    changeState(State::ERROR);
  }
}

void StateMachine::handleDisplaying() {
  if (source->readMessage(currentMessage)) {
    changeState(State::MESSAGE_RECEIVED);
    return;
  }

  if (millis() - stateEnteredAt >= Config::DISPLAY_DURATION_MS) {
    led->turnOff();
    display->showIdle();
    changeState(State::IDLE);
  }
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
}

const char* StateMachine::stateName(State state) {
  switch (state) {
    case State::IDLE:
      return "IDLE";
    case State::MESSAGE_RECEIVED:
      return "MESSAGE_RECEIVED";
    case State::DISPLAYING:
      return "DISPLAYING";
    case State::ERROR:
      return "ERROR";
  }
  return "UNKNOWN";
}
