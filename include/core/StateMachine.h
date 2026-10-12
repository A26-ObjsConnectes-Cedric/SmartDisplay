#pragma once

#include "outputs/Display.h"
#include "interfaces/IMessageSource.h"
#include "outputs/LedActuator.h"
#include "models/Message.h"

enum class State
 { 
    IDLE, 
    MESSAGE_RECEIVED, 
    DISPLAYING, ERROR 
};

// Logique applicative du panneau.
// Responsabilité : coordonner la source, l'affichage et la DEL selon l'état courant.
// La machine à états ne connaît que l'interface IMessageSource, pas le bouton.
class StateMachine {
 public:
  StateMachine(IMessageSource* source, Display* display, LedActuator* led);

  void begin();

  void update();

  State getState() const;

 private:
  State currentState;
  Message currentMessage;
  IMessageSource* source;
  Display* display;
  LedActuator* led;
};
