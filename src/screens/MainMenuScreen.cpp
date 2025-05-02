#include "ClickMessage.h"
#include "Colors.h"
#include "EntityHandle.h"
#include "EntityManager.h"
#include "Fonts.h"
#include "GameStateManager.h"
#include "GameStatsManager.h"
#include "MainMenuScreen.h"
#include "MouseMotionMessage.h"

MainMenuScreen::~MainMenuScreen() {
}

void MainMenuScreen::create(EntityManager* eManager, SDL_Renderer* renderer) {
    playTexture.m_renderer = renderer;
    settingsTexture.m_renderer = renderer;
    quitTexture.m_renderer = renderer;
    goldTexture.m_renderer = renderer;
    playTexture.loadFromText("Play", Colors::White, Fonts::Title);
    settingsTexture.loadFromText("Settings", Colors::White, Fonts::Title);
    quitTexture.loadFromText("Exit Game", Colors::White, Fonts::Title);
    std::ostringstream goldText;
    goldText << "gold: " << GameStatsManager::instance().getStats().coins;
    goldTexture.loadFromText(goldText.str(), Colors::White, Fonts::Body);
    
    TypeComponent type = EntityType::GUI;
    TransformComponent trComp;
    CollisionComponent colComp;
    RenderComponent textureComp;
    GUIComponent guiComp;
    // Play button
    playID = eManager->createEntity();
    EntityHandle eHandle = {playID, eManager};
    eHandle.add<RenderComponent>();
    eHandle.add<TypeComponent>();
    eHandle.add<GUIComponent>();
    eHandle.add<CollisionComponent>();
    trComp.position = FPair(
        calculateCenteredX(playTexture.getWidth()), SCREEN_HEIGHT / 3 - playTexture.getHeight());
    colComp.position = {trComp.position.x - hoveredOffset, trComp.position.y - hoveredOffset};
    colComp.height = playTexture.getHeight() + 2 * hoveredOffset;
    colComp.width = playTexture.getWidth() + 2 * hoveredOffset;
    textureComp.texture = &playTexture;
    eHandle.set<TransformComponent>(trComp);
    eHandle.set<TypeComponent>(type);
    eHandle.set<RenderComponent>(textureComp);
    eHandle.set<GUIComponent>(guiComp);
    eHandle.set<CollisionComponent>(colComp);
    //Settings
    settingsID = eManager->createEntity();
    eHandle.id = settingsID;
    eHandle.add<RenderComponent>();
    eHandle.add<TypeComponent>();
    eHandle.add<GUIComponent>();
    eHandle.add<CollisionComponent>();
    trComp.position.x = calculateCenteredX(settingsTexture.getWidth());
    trComp.position.y += playTexture.getHeight() + 2 * hoveredOffset;
    colComp.position = {trComp.position.x - hoveredOffset, trComp.position.y - hoveredOffset};
    colComp.height = settingsTexture.getHeight() + 2 * hoveredOffset;
    colComp.width = settingsTexture.getWidth() + 2 * hoveredOffset;
    textureComp.texture = &settingsTexture;
    eHandle.set<TransformComponent>(trComp);
    eHandle.set<TypeComponent>(type);
    eHandle.set<RenderComponent>(textureComp);
    eHandle.set<GUIComponent>(guiComp);
    eHandle.set<CollisionComponent>(colComp);
    //Quit
    quitID = eManager->createEntity();
    eHandle.id = quitID;
    eHandle.add<RenderComponent>();
    eHandle.add<TypeComponent>();
    eHandle.add<GUIComponent>();
    eHandle.add<CollisionComponent>();
    trComp.position.x = calculateCenteredX(quitTexture.getWidth());
    trComp.position.y += settingsTexture.getHeight() + 2 * hoveredOffset;
    colComp.position = {trComp.position.x - hoveredOffset, trComp.position.y - hoveredOffset};
    colComp.height = quitTexture.getHeight() + 2 * hoveredOffset;
    colComp.width = quitTexture.getWidth() + 2 * hoveredOffset;
    textureComp.texture = &quitTexture;
    eHandle.set<TransformComponent>(trComp);
    eHandle.set<TypeComponent>(type);
    eHandle.set<RenderComponent>(textureComp);
    eHandle.set<GUIComponent>(guiComp);
    eHandle.set<CollisionComponent>(colComp);
    //Global gold
    goldID = eManager->createEntity();
    eHandle.id = goldID;
    eHandle.add<RenderComponent>();
    eHandle.add<TypeComponent>();
    eHandle.add<GUIComponent>();
    trComp.position.x = SCREEN_WIDTH - goldTexture.getWidth() - 10.0f;
    trComp.position.y = SCREEN_HEIGHT - goldTexture.getHeight() - 10.0f;
    textureComp.texture = &goldTexture;
    eHandle.set<TransformComponent>(trComp);
    eHandle.set<TypeComponent>(type);
    eHandle.set<RenderComponent>(textureComp);
    eHandle.set<GUIComponent>(guiComp);
}

void MainMenuScreen::destroy(EntityManager* eManager) {
    eManager->destroyEntityLater(playID);
    eManager->destroyEntityLater(settingsID);
    eManager->destroyEntityLater(quitID);
}

void MainMenuScreen::handleMouseHover(std::shared_ptr<MouseMotionMessage> msg) {
    handleHover(playID, msg->mousePos, nullptr);
    handleHover(settingsID, msg->mousePos, nullptr);
    handleHover(quitID, msg->mousePos, nullptr);
}

void MainMenuScreen::handleMouseClick(std::shared_ptr<ClickMessage> msg) {
    handleClick(playID, msg->mousePos, [this]() {
        onPlayClick();
    });
    handleClick(quitID, msg->mousePos, [this]() {
        GameStateManager::instance().setState(GameState::Quit);
    });
}

void MainMenuScreen::onPlayClick() {
    GameStateManager::instance().setState(GameState::Restart);
}

void MainMenuScreen::update(EntityManager* eManager, SDL_Renderer* renderer) {

}
