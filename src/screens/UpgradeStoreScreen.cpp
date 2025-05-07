#include "texture.hpp"
#include "EntityManager.h"
#include "MetaStats.hpp"
#include "UpgradeStoreScreen.h"
#include "GameStatsManager.h"
#include "GameStateManager.h"

UpgradeStoreScreen::UpgradeStoreScreen(EntityManager* eManager, SDL_Renderer* renderer) : Screen(eManager) {
    pressedButtonTexture.m_renderer = renderer;
    idleButtonTexture.m_renderer = renderer;
    containerTexture.m_renderer = renderer;
    backButtonTexture.m_renderer = renderer;
    idleButtonTexture.loadFromFile("data/img/gui/buttonIdle.bmp");
    pressedButtonTexture.loadFromFile("data/img/gui/buttonPressed.bmp");
    backButtonTexture.loadFromFile("data/img/gui/backButton.bmp");
}

void UpgradeStoreScreen::create(EntityManager* eManager, SDL_Renderer* renderer) {
    //BackButton
    FPair size {backButtonTexture.getWidth(), backButtonTexture.getHeight()};
    FPair pos {40.0f, 40.0f};
    backButton = std::make_shared<Button>(eManager, "", pos, size, &backButtonTexture, renderer);
    ClickCallbackComponent callback;
    callback.onClick = [&] (uint32_t entity) {
        GameStateManager::instance().setState(GameState::MainMenu);
    };
    eManager->addComponent(backButton.get()->id, ComponentType::ClickCallback);
    eManager->setComponentData<ClickCallbackComponent>(backButton.get()->id, callback);
    // UpgradeButtons
    containerTexture.m_renderer = renderer;
    containerTexture.createEmptyTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
    for (int i = 0; i < static_cast<size_t>(UpgradeType::UpgradesCount); i++) {
        UpgradeType type = static_cast<UpgradeType>(i);
        size = {200.0f};
        pos = {i % 2 ? SCREEN_CENTER.x + 240 : SCREEN_CENTER.x + 20,
                   (i / 2) * size.y + 40};
        std::string label = to_string(type);
        upgradeButtons.push_back(std::make_shared<Button>(
            eManager, label, pos, size, &idleButtonTexture, renderer));
        int level = GameStatsManager::instance().getUpgradeLevel(type);
        callback.onClick = [&, type, level](uint32_t entity) {
            const int cost = upgradesCost[static_cast<size_t>(type)][level];
            if (GameStatsManager::instance().spendCoins(cost)) {
                GameStatsManager::instance().getStats().upgrades[type]++;
                GameStatsManager::instance().save("data/stats.json");
            }
        };
        eManager->addComponent(upgradeButtons[i].get()->id, ComponentType::ClickCallback);
        eManager->setComponentData<ClickCallbackComponent>(upgradeButtons[i].get()->id, callback);
    }
}

void UpgradeStoreScreen::destroy(EntityManager* eManager) {
    backButton.get()->destroy(eManager);
    for (auto b : upgradeButtons) {
        b.get()->destroy(eManager);
    }
}

void UpgradeStoreScreen::handleMouseHover(std::shared_ptr<MouseMotionMessage> msg) {

}

void UpgradeStoreScreen::handleMouseClick(std::shared_ptr<ClickMessage> msg) {

}

void UpgradeStoreScreen::update(EntityManager* eManager, SDL_Renderer* renderer) {
    for (auto b : upgradeButtons) {
        GUIState state = eManager->getComponentData<GUIStateComponent>(b.get()->id).state;
        RenderComponent render = eManager->getComponentData<RenderComponent>(b.get()->id);
        if (state == GUIState::Pressed) {
            render.texture = &pressedButtonTexture;
        } else if (state == GUIState::Idle) {
            render.texture = &idleButtonTexture;
        }
        eManager->setComponentData(b.get()->id, render);
    }
}
