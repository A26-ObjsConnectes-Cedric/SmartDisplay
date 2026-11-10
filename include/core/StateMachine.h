#pragma once

#include "interfaces/IEventPublisher.h"
#include "interfaces/IMessageSource.h"
#include "models/Message.h"
#include "outputs/Display.h"
#include "outputs/LedActuator.h"


enum class State 
{ 
  IDLE, 
  MESSAGE_RECEIVED, 
  DISPLAYING, ERROR 
};
class StateMachine {
 public:
  StateMachine(IMessageSource* source, Display* display, LedActuator* led);

  void begin();

  void update();

  State getState() const;

  void setPublisher(IEventPublisher* publisher);

  void publishState();

 private:
  void handleIdle();
  void handleMessageReceived();
  void handleDisplaying();
  void handleError();

  void changeState(State newState);

  static const char* stateName(State state);

  State currentState;
  Message currentMessage;
  IMessageSource* source;
  Display* display;
  LedActuator* led;
  IEventPublisher* publisher;
  unsigned long stateEnteredAt;
};
