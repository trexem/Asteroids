#include "texture.hpp"
#include "EntityManager.h"
#include "MetaStats.hpp"
#include "UpgradeStoreScreen.h"
#include "GameStatsManager.h"
#include "GameStateManager.h"
#include "GUI.h"
#include "TextureManager.h"

UpgradeStoreScreen::UpgradeStoreScreen(EntityManager& eManager, SDL_Renderer* renderer) : Screen(eManager) {
    pressedButtonTexture.m_renderer = renderer;
    idleButtonTexture.m_renderer = renderer;
    containerTexture.m_renderer = renderer;
    backButtonTexture.m_renderer = renderer;
    idleButtonTexture.loadFromFile("data/img/gui/buttonIdle.bmp");
    pressedButtonTexture.loadFromFile("data/img/gui/buttonPressed.bmp");
    backButtonTexture.loadFromFile("data/img/gui/backButton.bmp");
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
    containerTexture.m_renderer = renderer;
    containerTexture.createEmptyTexture(GUI::screenWidth, GUI::screenHeight);
    for (int i = 0; i < static_cast<size_t>(UpgradeType::UpgradesCount); i++) {
        UpgradeType type = static_cast<UpgradeType>(i);
        size = {200.0f};
        pos = {i % 2 ? GUI::screenCenter.x + 240 : GUI::screenCenter.x + 20,
                   (i / 2) * size.y + 40};
        std::string label = to_string(type);
        upgradeButtons.push_back(std::make_shared<UpgradeButton>(
            eManager, label, pos, size, TextureManager::instance().get("gui/buttonIdle"), renderer, type));
    }
}

void UpgradeStoreScreen::destroy(EntityManager& eManager) {
    backButton->destroy(eManager);
    for (auto b : upgradeButtons) {
        b->destroy(eManager);
    }
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
        RenderComponent render = eManager.getComponentData<RenderComponent>(b->id);
        if (state == GUIState::Pressed) {
            render.texture = &pressedButtonTexture;
        } else if (state == GUIState::Idle || state == GUIState::Hovered) {
            render.texture = &idleButtonTexture;
        }
        eManager.setComponentData(b->id, render);
    }
    backButton->updateState(eManager);
}
