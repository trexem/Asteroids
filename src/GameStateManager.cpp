#include "GameStateManager.h"

GameStateManager& GameStateManager::getInstance() {
    static GameStateManager instance;
    return instance;
}

void GameStateManager::setState(GameState newState) {
    if (state != newState) {
        state = newState;
        auto msg = std::make_shared<GameStateMessage>(state);
        MessageManager::getInstance().sendMessage(msg);
    }
}
