#include "MainMenuScreen.h"

MainMenuScreen::~MainMenuScreen() {
}

void MainMenuScreen::create(EntityManager* eManager, SDL_Renderer* renderer) {
    playTexture.m_renderer = renderer;
    settingsTexture.m_renderer = renderer;
    quitTexture.m_renderer = renderer;
    playTexture.loadFromText("Play", Colors::White, Fonts::Title);
    settingsTexture.loadFromText("Settings", Colors::White, Fonts::Title);
    quitTexture.loadFromText("Exit Game", Colors::White, Fonts::Title);
    
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
        GameStateManager::getInstance().setState(GameState::Quit);
    });
}

void MainMenuScreen::onPlayClick() {
    GameStateManager::getInstance().setState(GameState::Restart);
}

void MainMenuScreen::update(EntityManager* eManager, SDL_Renderer* renderer) {

}
