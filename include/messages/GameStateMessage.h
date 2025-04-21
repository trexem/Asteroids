#pragma once

#include "message.h"
#include "GameState.h"

class GameStateMessage : public Message {
public:
    GameState state;
    GameStateMessage(GameState s) : state(s) {} 
};
