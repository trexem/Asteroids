#include "ClickMessage.h"
#include "Colors.h"
#include "EntityHandle.h"
#include "EntityManager.h"
#include "Fonts.h"
#include "GameStateManager.h"
#include "GameStatsManager.h"
#include "GUI.h"
#include "SettingsManager.h"
#include "MainMenuScreen.h"
#include "MouseMotionMessage.h"
#include "screens/Components/Button.h"
#include "screens/Components/Label.h"
#include "texture.hpp"

MainMenuScreen::~MainMenuScreen() {
}

void MainMenuScreen::create(EntityManager& eManager, SDL_Renderer* renderer) {
    // Play button
    FPair pos {GUI::screenCenter.x - 250.0f, GUI::screenCenter.y - 200.0f};
    FPair size {500.0f, 100.0f};
    ClickCallbackComponent callback;
    callback.onClick = [&](uint32_t entity) {
        onPlayClick();
    };
    playButton = std::make_unique<Button>(eManager, "Play", pos, size, nullptr, renderer, 0, Fonts::Title);
    eManager.addComponent(playButton->id, ComponentType::ClickCallback);
    eManager.setComponentData(playButton->id, callback);
    //Upgrades
    pos.y += 100.0f;
    callback.onClick = [&](uint32_t entity) {
        GameStateManager::instance().setState(GameState::UpgradeStore);
    };
    upgradesButton = std::make_unique<Button>(eManager, "Upgrades", pos, size, nullptr, renderer, 0, Fonts::Title);
    eManager.addComponent(upgradesButton->id, ComponentType::ClickCallback);
    eManager.setComponentData(upgradesButton->id, callback);
    //Settings
    pos.y += 100.0f;
    callback.onClick = [&](uint32_t entity) {
        GameStateManager::instance().setState(GameState::Settings);
    };
    settingsButton = std::make_unique<Button>(eManager, "Settings", pos, size, nullptr, renderer, 0, Fonts::Title);
    eManager.addComponent(settingsButton->id, ComponentType::ClickCallback);
    eManager.setComponentData(settingsButton->id, callback);
    //Quit
    pos.y += 100.0f;
    callback.onClick = [&](uint32_t entity) {
        GameStateManager::instance().setState(GameState::Quit);
    };
    quitButton = std::make_unique<Button>(eManager, "Exit", pos, size, nullptr, renderer, 0, Fonts::Title);
    eManager.addComponent(quitButton->id, ComponentType::ClickCallback);
    eManager.setComponentData(quitButton->id, callback);
    //Global gold
    std::ostringstream goldText;
    goldText << "gold: " << GameStatsManager::instance().getStats().coins;
    pos.x = GUI::screenWidth - 200.0f;
    pos.y = GUI::screenHeight - 75.0f;
    goldLabel = std::make_unique<Label>(eManager, goldText.str(), pos, size, renderer);
}

void MainMenuScreen::destroy(EntityManager& eManager) {
    playButton->destroy(eManager);
    upgradesButton->destroy(eManager);
    settingsButton->destroy(eManager);
    quitButton->destroy(eManager);
    goldLabel->destroy(eManager);
}

void MainMenuScreen::handleMouseHover(std::shared_ptr<MouseMotionMessage> msg) {
}

void MainMenuScreen::handleMouseClick(std::shared_ptr<ClickMessage> msg) {
}

void MainMenuScreen::onPlayClick() {
    GameStateManager::instance().setState(GameState::Restart);
}

void MainMenuScreen::update(EntityManager& eManager, SDL_Renderer* renderer) {
    playButton->updateState(eManager);
    upgradesButton->updateState(eManager);
    settingsButton->updateState(eManager);
    quitButton->updateState(eManager);
}
