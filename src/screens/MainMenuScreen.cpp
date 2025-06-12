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
#include "TextManager.h"
#include "TextureManager.h"

MainMenuScreen::~MainMenuScreen() {
}

void MainMenuScreen::create(EntityManager& eManager, SDL_Renderer* renderer) {
    //Background
    FPair pos {0.0f, 0.0f};
    FPair size {1920.0f, 1080.0f};
    backgroundID = eManager.createEntity();
    EntityHandle handle{backgroundID, eManager};
    handle.add<GUIComponent>();
    handle.add<RenderComponent>();
    TypeComponent type = EntityType::GUI;
    GUIComponent guiComp;
    guiComp.pos = pos;
    guiComp.size = size;
    RenderComponent renderComp;
    renderComp.texture = TextureManager::instance().get("gui/mainMenuBG");
    handle.set(type);
    handle.set(guiComp);
    handle.set(renderComp);
    // Play button
    pos = {GUI::screenCenter.x - 400.0f, GUI::screenCenter.y - 50.0f};
    size = {800.0f, 100.0f};
    ClickCallbackComponent callback;
    callback.onClick = [&](uint32_t entity) {
        onPlayClick();
    };
    std::string text = TextManager::instance().get("action.play");
    playButton = std::make_unique<Button>(eManager, text, pos, size, nullptr, renderer, 0, Fonts::Title);
    eManager.addComponent(playButton->id, ComponentType::ClickCallback);
    eManager.setComponentData(playButton->id, callback);
    //Upgrades
    pos.y += 120.0f;
    callback.onClick = [&](uint32_t entity) {
        GameStateManager::instance().setState(GameState::UpgradeStore);
    };
    text = TextManager::instance().get("action.upgrades");
    upgradesButton = std::make_unique<Button>(eManager, text, pos, size, nullptr, renderer, 0, Fonts::Title);
    eManager.addComponent(upgradesButton->id, ComponentType::ClickCallback);
    eManager.setComponentData(upgradesButton->id, callback);
    //Settings
    pos.y += 120.0f;
    callback.onClick = [&](uint32_t entity) {
        GameStateManager::instance().setState(GameState::Settings);
    };
    text = TextManager::instance().get("action.settings");
    settingsButton = std::make_unique<Button>(eManager, text, pos, size, nullptr, renderer, 0, Fonts::Title);
    eManager.addComponent(settingsButton->id, ComponentType::ClickCallback);
    eManager.setComponentData(settingsButton->id, callback);
    //Quit
    pos.y += 120.0f;
    callback.onClick = [&](uint32_t entity) {
        GameStateManager::instance().setState(GameState::Quit);
    };
    text = TextManager::instance().get("action.quit");
    quitButton = std::make_unique<Button>(eManager, text, pos, size, nullptr, renderer, 0, Fonts::Title);
    eManager.addComponent(quitButton->id, ComponentType::ClickCallback);
    eManager.setComponentData(quitButton->id, callback);
    //Global gold
    text = TextManager::instance().format("label.gold", GameStatsManager::instance().getStats().coins);
    pos.x = GUI::screenWidth - 300.0f;
    pos.y += 100.0f;
    goldLabel = std::make_unique<Label>(eManager, text, pos, size, renderer);
    //Max Kills
    text = TextManager::instance().format("label.maxKills", GameStatsManager::instance().getStats().maxKills);
    pos.x = 150.0f;
    pos.y = GUI::screenHeight - 200.0f;
    maxKills = std::make_unique<Label>(eManager, text, pos, size, renderer);
    //Max Level
    text = TextManager::instance().format("label.maxLevel", GameStatsManager::instance().getStats().maxLevel);
    pos.y += 50.0f;
    maxLevel = std::make_unique<Label>(eManager, text, pos, size, renderer);
    //Max Time
    std::string time = formatTimeMMSS(GameStatsManager::instance().getStats().maxTime);
    text = TextManager::instance().format("label.maxTime", time);
    pos.y += 50.0f;
    maxTime = std::make_unique<Label>(eManager, text, pos, size, renderer);
}

void MainMenuScreen::destroy(EntityManager& eManager) {
    playButton->destroy(eManager);
    upgradesButton->destroy(eManager);
    settingsButton->destroy(eManager);
    quitButton->destroy(eManager);
    goldLabel->destroy(eManager);
    eManager.destroyEntityLater(backgroundID);
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
