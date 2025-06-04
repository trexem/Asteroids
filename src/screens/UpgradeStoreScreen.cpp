#include "texture.hpp"
#include "EntityManager.h"
#include "MetaStats.hpp"
#include "UpgradeStoreScreen.h"
#include "GameStatsManager.h"
#include "GameStateManager.h"
#include "GUI.h"
#include "TextureManager.h"
#include "TextManager.h"

UpgradeStoreScreen::UpgradeStoreScreen(EntityManager& eManager, SDL_Renderer* renderer) : Screen(eManager) {
}

void UpgradeStoreScreen::create(EntityManager& eManager, SDL_Renderer* renderer) {
    //BackButton
    Texture* backTexture = TextureManager::instance().get("gui/backButton");
    FPair size {backTexture->getWidth(), backTexture->getHeight()};
    FPair pos {40.0f, 40.0f};
    backButton = std::make_shared<Button>(eManager, "", pos, size, backTexture, renderer);
    ClickCallbackComponent callback;
    callback.onClick = [&] (uint32_t entity) {
        GameStateManager::instance().setState(GameState::MainMenu);
    };
    eManager.addComponent(backButton.get()->id, ComponentType::ClickCallback);
    eManager.setComponentData<ClickCallbackComponent>(backButton.get()->id, callback);
    // UpgradeButtons
    for (int i = 0; i < static_cast<size_t>(UpgradeType::UpgradesCount); i++) {
        UpgradeType type = static_cast<UpgradeType>(i);
        size = 200.0f;
        pos = {(i / 2) * (size.x + 40.0f) + 240.0f, 
            i % 2 ? GUI::screenCenter.y + 20.0f : GUI::screenCenter.y - 220.0f};
        std::string key = to_key(type);
        std::string text = TextManager::instance().get("upgradeName." + key);
        upgradeButtons.push_back(std::make_shared<UpgradeButton>(
            eManager, text, pos, size, TextureManager::instance().get("gui/buttonIdle"), renderer, type));
    }
    //Global gold
    gold = GameStatsManager::instance().getStats().coins;
    std::string goldText = TextManager::instance().format("label.gold", gold);
    pos.x = GUI::screenWidth - 200.0f;
    pos.y = GUI::screenHeight - 75.0f;
    goldLabel = std::make_unique<Label>(eManager, goldText, pos, size, renderer);
}

void UpgradeStoreScreen::destroy(EntityManager& eManager) {
    backButton->destroy(eManager);
    for (auto b : upgradeButtons) {
        b->destroy(eManager);
    }
    goldLabel->destroy(eManager);
}

void UpgradeStoreScreen::handleMouseHover(std::shared_ptr<MouseMotionMessage> msg) {

}

void UpgradeStoreScreen::handleMouseClick(std::shared_ptr<ClickMessage> msg) {

}

void UpgradeStoreScreen::update(EntityManager& eManager, SDL_Renderer* renderer) {
    for (auto b : upgradeButtons) {
        b->updateCost(eManager);
        b->updateState(eManager);
        GUIState state = eManager.getComponentData<GUIStateComponent>(b->id).state;
        RenderComponent render = eManager.getComponentData<RenderComponent>(b->box->id);
        eManager.setComponentData(b->box->id, render);
    }
    backButton->updateState(eManager);
    uint32_t newGold = GameStatsManager::instance().getStats().coins;
    if (gold != newGold) {
        gold = newGold;
        std::string goldText = TextManager::instance().format("label.gold", gold);
        goldLabel->setText(goldText);
    }
}
