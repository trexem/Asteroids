#pragma once
#ifndef __GAME_STATE_MANAGER_H_
#define __GAME_STATE_MANAGER_H_

#include "GameStateMessage.h"
#include "message.h"
#include "MessageManager.h"

class GameStateManager {
public:
    static GameStateManager& getInstance();

    void setState(GameState newState);

    GameState getState() const { return state; }

private:
    GameState state{GameState::MainMenu};
    GameStateManager() = default;
    ~GameStateManager() = default;

    GameStateManager(const GameStateManager&) = delete;
    GameStateManager& operator=(const GameStateManager&) = delete;
};

#endif // !__GAME_STATE_MANAGER_H_
