#include "MainMenuScreen.h"

MainMenuScreen::~MainMenuScreen() {
    // std::cout << "Destroying MainMenuScreen textures\n";
    // playTexture.free();
    // settingsTexture.free();
    // quitTexture.free();
}

void MainMenuScreen::create(EntityManager* eManager, SDL_Renderer* renderer) {
    playTexture.m_renderer = renderer;
    settingsTexture.m_renderer = renderer;
    quitTexture.m_renderer = renderer;
    playTexture.loadFromText("Play", Colors::White, Fonts::Title);
    settingsTexture.loadFromText("Settings", Colors::White, Fonts::Title);
    quitTexture.loadFromText("Exit", Colors::White, Fonts::Title);
    
    TypeComponent type = EntityType::GUI;
    TransformComponent trComp;
    CollisionComponent colComp;
    RenderComponent textureComp;
    GUIComponent guicomp;
    // Play button
    playID = eManager->createEntity();
    eManager->addComponent(playID, ComponentType::Render);
    eManager->addComponent(playID, ComponentType::Type);
    eManager->addComponent(playID, ComponentType::Collision);
    eManager->addComponent(playID, ComponentType::GUI);
    trComp.position = FPair(
        calculateCenteredX(playTexture.getWidth()), SCREEN_HEIGHT / 3 - playTexture.getHeight());
    colComp.position = {trComp.position.x - hoveredOffset, trComp.position.y - hoveredOffset};
    colComp.height = playTexture.getHeight() + 2 * hoveredOffset;
    colComp.width = playTexture.getWidth() + 2 * hoveredOffset;
    textureComp.texture = &playTexture;
    eManager->setComponentData<TransformComponent>(playID, trComp);
    eManager->setComponentData<TypeComponent>(playID, type);
    eManager->setComponentData<CollisionComponent>(playID, colComp);
    eManager->setComponentData<RenderComponent>(playID, textureComp);
    eManager->setComponentData<GUIComponent>(playID, guicomp);
    //Settings
    settingsID = eManager->createEntity();
    eManager->addComponent(settingsID, ComponentType::Render);
    eManager->addComponent(settingsID, ComponentType::Type);
    eManager->addComponent(settingsID, ComponentType::Collision);
    eManager->addComponent(settingsID, ComponentType::GUI);
    trComp.position.x = calculateCenteredX(settingsTexture.getWidth());
    trComp.position.y += playTexture.getHeight() + 2 * hoveredOffset;
    colComp.position = {trComp.position.x - hoveredOffset, trComp.position.y - hoveredOffset};
    colComp.height = settingsTexture.getHeight() + 2 * hoveredOffset;
    colComp.width = settingsTexture.getWidth() + 2 * hoveredOffset;
    textureComp.texture = &settingsTexture;
    eManager->setComponentData<TransformComponent>(settingsID, trComp);
    eManager->setComponentData<TypeComponent>(settingsID, type);
    eManager->setComponentData<CollisionComponent>(settingsID, colComp);
    eManager->setComponentData<RenderComponent>(settingsID, textureComp);
    eManager->setComponentData<GUIComponent>(settingsID, guicomp);
    //Quit
    quitID = eManager->createEntity();
    eManager->addComponent(quitID, ComponentType::Render);
    eManager->addComponent(quitID, ComponentType::Type);
    eManager->addComponent(quitID, ComponentType::Collision);
    eManager->addComponent(quitID, ComponentType::GUI);
    trComp.position.x = calculateCenteredX(quitTexture.getWidth());
    trComp.position.y += settingsTexture.getHeight() + 2 * hoveredOffset;
    colComp.position = {trComp.position.x - hoveredOffset, trComp.position.y - hoveredOffset};
    colComp.height = quitTexture.getHeight() + 2 * hoveredOffset;
    colComp.width = quitTexture.getWidth() + 2 * hoveredOffset;
    textureComp.texture = &quitTexture;
    eManager->setComponentData<TransformComponent>(quitID, trComp);
    eManager->setComponentData<TypeComponent>(quitID, type);
    eManager->setComponentData<CollisionComponent>(quitID, colComp);
    eManager->setComponentData<RenderComponent>(quitID, textureComp);
    eManager->setComponentData<GUIComponent>(quitID, guicomp);
}

void MainMenuScreen::destroy(EntityManager* eManager) {
    std::cout << "Destroying MainMenuScreen resources...\n";
    eManager->destroyEntity(playID);
    eManager->destroyEntity(settingsID);
    eManager->destroyEntity(quitID);
    std::cout << "Freeing textures...\n";
    // playTexture.free();
    // settingsTexture.free();
    // quitTexture.free();
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
}

void MainMenuScreen::onPlayClick() {
    GameStateManager::getInstance().setState(GameState::Playing);
}

void MainMenuScreen::update(EntityManager* eManager, SDL_Renderer* renderer) {

}
