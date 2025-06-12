#pragma once

#include "GameStateMessage.h"
#include "message.h"
#include "MessageManager.h"
#include "timer.hpp"

#include <iostream>

class KeyboardMessage;

class GameStateManager {
public:
    static GameStateManager& instance();

    void setState(GameState newState);
    void startTimer();
    Uint32 getGameTimeSeconds();

    GameState getState() const { return state; }
    GameState getLastState() const { return lastState; }

private:
    GameState state{GameState::Quit};
    GameState lastState{GameState::Quit};
    Timer timer;
    GameStateManager();
    ~GameStateManager() = default;


    GameStateManager(const GameStateManager&) = delete;
    GameStateManager& operator=(const GameStateManager&) = delete;

    void handleKeyboardMessage(std::shared_ptr<KeyboardMessage> msg);
};
