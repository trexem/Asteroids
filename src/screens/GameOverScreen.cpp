#include "ClickMessage.h"
#include "EntityHandle.h"
#include "EntityManager.h"
#include "GameOverScreen.h"
#include "GameStateManager.h"
#include "GameStatsManager.h"
#include "MouseMotionMessage.h"
#include "GUI.h"
#include "TextManager.h"

void GameOverScreen::create(EntityManager& eManager, SDL_Renderer* renderer) {
    //Restart
    FPair pos {GUI::screenCenter.x - 450.0f, GUI::screenCenter.y - 200.0f};
    FPair size {900.0f, 100.0f};
    ClickCallbackComponent callback;
    callback.onClick = [&](uint32_t entity) {
        GameStateManager::instance().setState(GameState::Restart);
    };
    std::string text = TextManager::instance().get("action.restart");
    restartButton = std::make_unique<Button>(eManager, text, pos, size, nullptr, renderer, 0, Fonts::Title);
    eManager.addComponent(restartButton->id, ComponentType::ClickCallback);
    eManager.setComponentData(restartButton->id, callback);

    //Main Menu
    pos.y += 100.0f;
    callback.onClick = [&](uint32_t entity) {
        GameStateManager::instance().setState(GameState::MainMenu);
    };
    text = TextManager::instance().get("action.mainMenu");
    mainMenuButton = std::make_unique<Button>(eManager, text, pos, size, nullptr, renderer, 0, Fonts::Title);
    eManager.addComponent(mainMenuButton->id, ComponentType::ClickCallback);
    eManager.setComponentData(mainMenuButton->id, callback);

    //Exit Game
    pos.y += 100.0f;
    callback.onClick = [&](uint32_t entity) {
        GameStateManager::instance().setState(GameState::Quit);
    };
    text = TextManager::instance().get("action.quit");
    exitButton = std::make_unique<Button>(eManager, text, pos, size, nullptr, renderer, 0, Fonts::Title);
    eManager.addComponent(exitButton->id, ComponentType::ClickCallback);
    eManager.setComponentData(exitButton->id, callback);
}

void GameOverScreen::destroy(EntityManager& eManager) {
    restartButton->destroy(eManager);
    mainMenuButton->destroy(eManager);
    exitButton->destroy(eManager);
    
}

void GameOverScreen::handleMouseHover(std::shared_ptr<MouseMotionMessage> msg) {
}

void GameOverScreen::handleMouseClick(std::shared_ptr<ClickMessage> msg) {
}

void GameOverScreen::update(EntityManager& eManager, SDL_Renderer* renderer) {
    restartButton->updateState(eManager);
    mainMenuButton->updateState(eManager);
    exitButton->updateState(eManager);
}
