#include "EntityHandle.h"
#include "GameStateManager.h"
#include "PauseScreen.h"

void PauseScreen::create(EntityManager* eManager, SDL_Renderer* renderer) {
    // Continue
    FPair pos {SCREEN_CENTER.x - 250.0f, SCREEN_CENTER.y - 200.0f};
    FPair size {500.0f, 100.0f};
    ClickCallbackComponent callback;
    callback.onClick = [&](uint32_t entity) {
        GameStateManager::instance().setState(GameState::Playing);
    };
    continueButton = std::make_unique<Button>(eManager, "Continue", pos, size, nullptr, renderer, 0, Fonts::Title);
    eManager->addComponent(continueButton->id, ComponentType::ClickCallback);
    eManager->setComponentData(continueButton->id, callback);
    // Settings
    pos.y += 100.0f;
    callback.onClick = [&](uint32_t entity) {
        GameStateManager::instance().setState(GameState::Settings);
    };
    settingsButton = std::make_unique<Button>(eManager, "Settings", pos, size, nullptr, renderer, 0, Fonts::Title);
    eManager->addComponent(settingsButton->id, ComponentType::ClickCallback);
    eManager->setComponentData(settingsButton->id, callback);
    // Main Menu
    pos.y += 100.0f;
    callback.onClick = [&](uint32_t entity) {
        GameStateManager::instance().setState(GameState::MainMenu);
    };
    mainMenuButton = std::make_unique<Button>(eManager, "Main Menu", pos, size, nullptr, renderer, 0, Fonts::Title);
    eManager->addComponent(mainMenuButton->id, ComponentType::ClickCallback);
    eManager->setComponentData(mainMenuButton->id, callback);
    // Exit
    pos.y += 100.0f;
    callback.onClick = [&](uint32_t entity) {
        GameStateManager::instance().setState(GameState::Quit);
    };
    exitButton = std::make_unique<Button>(eManager, "Exit", pos, size, nullptr, renderer, 0, Fonts::Title);
    eManager->addComponent(exitButton->id, ComponentType::ClickCallback);
    eManager->setComponentData(exitButton->id, callback);
}

void PauseScreen::destroy(EntityManager* eManager) {
    continueButton->destroy(eManager);
    settingsButton->destroy(eManager);
    mainMenuButton->destroy(eManager);
    exitButton->destroy(eManager);
}

void PauseScreen::handleMouseHover(std::shared_ptr<MouseMotionMessage> msg) {

}

void PauseScreen::handleMouseClick(std::shared_ptr<ClickMessage> msg) {

}

void PauseScreen::update(EntityManager* eManager, SDL_Renderer* renderer) {
    continueButton->updateState(eManager);
    settingsButton->updateState(eManager);
    mainMenuButton->updateState(eManager);
    exitButton->updateState(eManager);
}
