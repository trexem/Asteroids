#include "ClickMessage.h"
#include "EntityHandle.h"
#include "EntityManager.h"
#include "GameOverScreen.h"
#include "GameStateManager.h"
#include "GameStatsManager.h"
#include "MouseMotionMessage.h"
#include "GUI.h"

void GameOverScreen::create(EntityManager* eManager, SDL_Renderer* renderer) {
    restartTexture.m_renderer = renderer;
    mainMenuTexture.m_renderer = renderer;
    quitTexture.m_renderer = renderer;
    restartTexture.loadFromText("Restart", Colors::White, Fonts::Title);
    mainMenuTexture.loadFromText("Main Menu", Colors::White, Fonts::Title);
    quitTexture.loadFromText("Exit Game", Colors::White, Fonts::Title);

    TypeComponent type = EntityType::GUI;
    TransformComponent trComp;
    CollisionComponent cComp;
    RenderComponent rComp;
    GUIComponent gComp;
    // gComp.clickable = true;
    // gComp.hoverable = true;

    //Restart
    restartID = eManager->createEntity();
    EntityHandle eHandle = {restartID, eManager};
    eHandle.add<RenderComponent>();
    eHandle.add<TypeComponent>();
    eHandle.add<GUIComponent>();
    eHandle.add<CollisionComponent>();
    gComp.pos = FPair(
        calculateCenteredX(restartTexture.getWidth()), GUI::screenHeight / 3 - restartTexture.getHeight());
    cComp.position = {gComp.pos.x - hoveredOffset, gComp.pos.y - hoveredOffset};
    cComp.height = restartTexture.getHeight() + 2 * hoveredOffset;
    cComp.width = restartTexture.getWidth() + 2 * hoveredOffset;
    rComp.texture = &restartTexture;
    eHandle.set<TransformComponent>(trComp);
    eHandle.set<TypeComponent>(type);
    eHandle.set<RenderComponent>(rComp);
    eHandle.set<GUIComponent>(gComp);
    eHandle.set<CollisionComponent>(cComp);

    //Main Menu
    mmID = eManager->createEntity();
    eHandle.id = mmID;
    eHandle.add<RenderComponent>();
    eHandle.add<TypeComponent>();
    eHandle.add<GUIComponent>();
    eHandle.add<CollisionComponent>();
    gComp.pos.x = calculateCenteredX(mainMenuTexture.getWidth());
    gComp.pos.y += cComp.height + 2 * hoveredOffset;
    cComp.position = {gComp.pos.x - hoveredOffset, gComp.pos.y - hoveredOffset};
    cComp.height = mainMenuTexture.getHeight() + 2 * hoveredOffset;
    cComp.width = mainMenuTexture.getWidth() + 2 * hoveredOffset;
    rComp.texture = &mainMenuTexture;
    eHandle.set<TransformComponent>(trComp);
    eHandle.set<TypeComponent>(type);
    eHandle.set<RenderComponent>(rComp);
    eHandle.set<GUIComponent>(gComp);
    eHandle.set<CollisionComponent>(cComp);

    //Exit Game
    quitID = eManager->createEntity();
    eHandle.id = quitID;
    eHandle.add<RenderComponent>();
    eHandle.add<TypeComponent>();
    eHandle.add<GUIComponent>();
    eHandle.add<CollisionComponent>();
    gComp.pos.x = calculateCenteredX(quitTexture.getWidth());
    gComp.pos.y += cComp.height + 2 * hoveredOffset;
    cComp.position = {gComp.pos.x - hoveredOffset, gComp.pos.y - hoveredOffset};
    cComp.height = quitTexture.getHeight() + 2 * hoveredOffset;
    cComp.width = quitTexture.getWidth() + 2 * hoveredOffset;
    rComp.texture = &quitTexture;
    eHandle.set<TransformComponent>(trComp);
    eHandle.set<TypeComponent>(type);
    eHandle.set<RenderComponent>(rComp);
    eHandle.set<GUIComponent>(gComp);
    eHandle.set<CollisionComponent>(cComp);
}

void GameOverScreen::destroy(EntityManager* eManager) {
    eManager->destroyEntityLater(restartID);
    eManager->destroyEntityLater(mmID);
    eManager->destroyEntityLater(quitID);
    
}

void GameOverScreen::handleMouseHover(std::shared_ptr<MouseMotionMessage> msg) {
    handleHover(restartID, msg->mousePos, nullptr);
    handleHover(mmID, msg->mousePos, nullptr);
    handleHover(quitID, msg->mousePos, nullptr);
}

void GameOverScreen::handleMouseClick(std::shared_ptr<ClickMessage> msg) {
    handleClick(restartID, msg->mousePos, [this]() {
        GameStateManager::instance().setState(GameState::Restart);
    });
    handleClick(mmID, msg->mousePos, [this]() {
        GameStateManager::instance().setState(GameState::MainMenu);
    });
    handleClick(quitID, msg->mousePos, [this]() {
        GameStateManager::instance().setState(GameState::Quit);
    });
}

void GameOverScreen::update(EntityManager* eManager, SDL_Renderer* renderer) {
    
}
