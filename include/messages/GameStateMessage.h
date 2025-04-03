#pragma once
#ifndef __GAME_STATE_MESSAGE_H_
#define __GAME_STATE_MESSAGE_H_

#include "message.h"
#include "GameState.h"

class GameStateMessage : public Message {
public:
    GameState state;
    GameStateMessage(GameState s) : state(s) {} 
};

#endif // !__GAME_STATE_MESSAGE_H_