#include "KeyboardMessage.h"
#include "GameStateManager.h"

GameStateManager::GameStateManager() {
    MessageManager::instance().subscribe<KeyboardMessage>(
        [this](std::shared_ptr<KeyboardMessage> msg) { handleKeyboardMessage(msg); }
    );
}

GameStateManager& GameStateManager::instance() {
    static GameStateManager instance;
    return instance;
}

void GameStateManager::setState(GameState newState) {
    std::cout << "Setting state to: " << newState << std::endl;
    if (state != newState) {
        lastState = state;
        state = newState;
        if (state != GameState::Playing) {
            timer.pause();
        } else {
            timer.unpause();
        }
        auto msg = std::make_shared<GameStateMessage>(state);
        MessageManager::instance().sendMessage(msg);
    }
}

void GameStateManager::startTimer() {
    timer.start();
}

Uint32 GameStateManager::getGameTimeSeconds() {
    return timer.getTicks() / 1000.0f;
}

void GameStateManager::handleKeyboardMessage(std::shared_ptr<KeyboardMessage> msg) {
    if (state == GameState::Playing && msg->key == SDLK_P) setState(GameState::Paused);
}
