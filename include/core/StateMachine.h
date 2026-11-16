#pragma once

#include "interfaces/IConfigSource.h"
#include "interfaces/IEventPublisher.h"
#include "interfaces/IMessageSource.h"
#include "models/Message.h"
#include "outputs/Display.h"
#include "outputs/LedActuator.h"

enum class State 
{ 
  IDLE, 
  MESSAGE_RECEIVED, 
  DISPLAYING, 
  ACK_SENT, 
  CONFIGURING, 
  ERROR 
};

class StateMachine {
 public:
  StateMachine(IMessageSource* source, Display* display, LedActuator* led);

  void begin();

  void update();

  State getState() const;

  void setPublisher(IEventPublisher* publisher);

  void setConfigSource(IConfigSource* configSource);

  void publishState();

  unsigned long getDisplayDurationMs() const;
  uint8_t getBrightnessPct() const;

 private:
  void handleIdle();
  void handleMessageReceived();
  void handleDisplaying();
  void handleAckSent();
  void handleConfiguring();
  void handleError();

  void changeState(State newState);

  static const char* stateName(State state);

  State currentState;
  Message currentMessage;
  PanelConfig pendingConfig;
  IMessageSource* source;
  IConfigSource* configSource;
  Display* display;
  LedActuator* led;
  IEventPublisher* publisher;
  unsigned long stateEnteredAt;
  unsigned long displayStartedAt;  
  unsigned long displayDurationMs;
  uint8_t brightnessPct;
};
