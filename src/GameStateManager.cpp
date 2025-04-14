#include "GameStateManager.h"

GameStateManager& GameStateManager::getInstance() {
    static GameStateManager instance;
    return instance;
}

void GameStateManager::setState(GameState newState) {
    if (state != newState) {
        state = newState;
        if (state != GameState::Playing) {
            timer.pause();
        } else {
            timer.unpause();
        }
        auto msg = std::make_shared<GameStateMessage>(state);
        MessageManager::getInstance().sendMessage(msg);
    }
}

void GameStateManager::startTimer() {
    timer.start();
}

Uint32 GameStateManager::getGameTimeSeconds() {
    return timer.getTicks() / 1000.0f;
}
