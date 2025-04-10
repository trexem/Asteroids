#pragma once
#ifndef __GAME_STATE_MANAGER_H_
#define __GAME_STATE_MANAGER_H_

#include "GameStateMessage.h"
#include "message.h"
#include "MessageManager.h"
#include "timer.hpp"

class GameStateManager {
public:
    static GameStateManager& getInstance();

    void setState(GameState newState);
    void startTimer();
    Uint32 getGameTimeSeconds();

    GameState getState() const { return state; }

private:
    GameState state{GameState::Quit};
    Timer timer;
    GameStateManager() = default;
    ~GameStateManager() = default;

    GameStateManager(const GameStateManager&) = delete;
    GameStateManager& operator=(const GameStateManager&) = delete;
};

#endif // !__GAME_STATE_MANAGER_H_
