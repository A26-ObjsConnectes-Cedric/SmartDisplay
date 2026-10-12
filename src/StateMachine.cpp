#include "core/StateMachine.h"

StateMachine::StateMachine(IMessageSource* source, Display* display, LedActuator* led)
    : currentState(State::IDLE),
      currentMessage(),
      source(source),
      display(display),
      led(led) {}

void StateMachine::begin() {
  currentState = State::IDLE;
  display->showIdle();
}

void StateMachine::update() {
  
  switch (currentState) {
    case State::IDLE:
    case State::MESSAGE_RECEIVED:
    case State::DISPLAYING:
    case State::ERROR:
      break;
  }
}

State StateMachine::getState() const 
{ 
    return currentState; 
}
