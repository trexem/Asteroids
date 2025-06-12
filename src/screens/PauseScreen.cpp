#include "EntityHandle.h"
#include "GameStateManager.h"
#include "PauseScreen.h"
#include "GUI.h"
#include "TextManager.h"

void PauseScreen::create(EntityManager& eManager, SDL_Renderer* renderer) {
    // Continue
    FPair pos {GUI::screenCenter.x - 450.0f, GUI::screenCenter.y - 200.0f};
    FPair size {900.0f, 100.0f};
    ClickCallbackComponent callback;
    callback.onClick = [&](uint32_t entity) {
        GameStateManager::instance().setState(GameState::Playing);
    };
    std::string text = TextManager::instance().get("action.continue");
    continueButton = std::make_unique<Button>(eManager, text, pos, size, nullptr, renderer, 0, Fonts::Title);
    eManager.addComponent(continueButton->id, ComponentType::ClickCallback);
    eManager.setComponentData(continueButton->id, callback);
    // Restart
    pos.y += 100.0f;
    callback.onClick = [&](uint32_t entity) {
        GameStateManager::instance().setState(GameState::Restart);
    };
    text = TextManager::instance().get("action.restart");
    restartButton = std::make_unique<Button>(eManager, text, pos, size, nullptr, renderer, 0, Fonts::Title);
    eManager.addComponent(restartButton->id, ComponentType::ClickCallback);
    eManager.setComponentData(restartButton->id, callback);
    // Settings
    pos.y += 100.0f;
    callback.onClick = [&](uint32_t entity) {
        GameStateManager::instance().setState(GameState::Settings);
    };
    text = TextManager::instance().get("action.settings");
    settingsButton = std::make_unique<Button>(eManager, text, pos, size, nullptr, renderer, 0, Fonts::Title);
    eManager.addComponent(settingsButton->id, ComponentType::ClickCallback);
    eManager.setComponentData(settingsButton->id, callback);
    // Main Menu
    pos.y += 100.0f;
    callback.onClick = [&](uint32_t entity) {
        GameStateManager::instance().setState(GameState::MainMenu);
    };
    text = TextManager::instance().get("action.mainMenu");
    mainMenuButton = std::make_unique<Button>(eManager, text, pos, size, nullptr, renderer, 0, Fonts::Title);
    eManager.addComponent(mainMenuButton->id, ComponentType::ClickCallback);
    eManager.setComponentData(mainMenuButton->id, callback);
    // Exit
    pos.y += 100.0f;
    callback.onClick = [&](uint32_t entity) {
        GameStateManager::instance().setState(GameState::Quit);
    };
    text = TextManager::instance().get("action.quit");
    exitButton = std::make_unique<Button>(eManager, text, pos, size, nullptr, renderer, 0, Fonts::Title);
    eManager.addComponent(exitButton->id, ComponentType::ClickCallback);
    eManager.setComponentData(exitButton->id, callback);
}

void PauseScreen::destroy(EntityManager& eManager) {
    continueButton->destroy(eManager);
    restartButton->destroy(eManager);
    settingsButton->destroy(eManager);
    mainMenuButton->destroy(eManager);
    exitButton->destroy(eManager);
}

void PauseScreen::handleMouseHover(std::shared_ptr<MouseMotionMessage> msg) {

}

void PauseScreen::handleMouseClick(std::shared_ptr<ClickMessage> msg) {

}

void PauseScreen::update(EntityManager& eManager, SDL_Renderer* renderer) {
    continueButton->updateState(eManager);
    restartButton->updateState(eManager);
    settingsButton->updateState(eManager);
    mainMenuButton->updateState(eManager);
    exitButton->updateState(eManager);
}
