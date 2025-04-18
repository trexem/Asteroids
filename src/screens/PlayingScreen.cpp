#include "PlayingScreen.h"

PlayingScreen::~PlayingScreen() {
    std::cout << "PlayingScreen destroyed\n";
}

void PlayingScreen::create(EntityManager* eManager, SDL_Renderer* renderer) {
    GameStateManager::getInstance().startTimer();
    
    xpContainerTexture.m_renderer = renderer;
    currentXpTexture.m_renderer = renderer;
    abilitiesContainersTexture.m_renderer = renderer;
    timeTexture.m_renderer = renderer;
    levelTexture.m_renderer = renderer;
    pauseTexture.m_renderer = renderer;
    drawAbilityContainers(renderer);
    drawXpContainer(renderer);
    currentXpTexture.createEmptyTexture(xpContainerTexture.getWidth() - 4, xpContainerTexture.getHeight() - 2);
    timeTexture.loadFromText("00:00", Colors::White, Fonts::Subtitle);
    levelTexture.loadFromText("lvl: 1" , Colors::White, Fonts::Body);
    pauseTexture.loadFromFile("data/img/pause.bmp");

    TypeComponent type = EntityType::GUI;
    GUIComponent guiComponent;
    TransformComponent trComp;
    CollisionComponent colComp;
    RenderComponent textureComp;
    //XP Container
    xpContainerID = eManager->createEntity();
    eManager->addComponent(xpContainerID, ComponentType::Render);
    eManager->addComponent(xpContainerID, ComponentType::Type);
    eManager->addComponent(xpContainerID, ComponentType::GUI);
    trComp.position = FPair(3, 3);
    textureComp.texture = &xpContainerTexture;
    eManager->setComponentData<TransformComponent>(xpContainerID, trComp);
    eManager->setComponentData<TypeComponent>(xpContainerID, type);
    eManager->setComponentData<RenderComponent>(xpContainerID, textureComp);
    eManager->setComponentData<GUIComponent>(xpContainerID, guiComponent);
    //Current XP
    currentXpID = eManager->createEntity();
    eManager->addComponent(currentXpID, ComponentType::Render);
    eManager->addComponent(currentXpID, ComponentType::Type);
    eManager->addComponent(currentXpID, ComponentType::GUI);
    trComp.position = FPair(4, 4);
    textureComp.texture = &currentXpTexture;
    eManager->setComponentData<TransformComponent>(currentXpID, trComp);
    eManager->setComponentData<TypeComponent>(currentXpID, type);
    eManager->setComponentData<RenderComponent>(currentXpID, textureComp);
    eManager->setComponentData<GUIComponent>(currentXpID, guiComponent);
    //Abilities Container
    abilitiesContainersID = eManager->createEntity();
    eManager->addComponent(abilitiesContainersID, ComponentType::Render);
    eManager->addComponent(abilitiesContainersID, ComponentType::GUI);
    eManager->addComponent(abilitiesContainersID, ComponentType::Type);
    trComp.position = FPair(3, 46);
    textureComp.texture = &abilitiesContainersTexture;
    eManager->setComponentData<TransformComponent>(abilitiesContainersID, trComp);
    eManager->setComponentData<TypeComponent>(abilitiesContainersID, type);
    eManager->setComponentData<RenderComponent>(abilitiesContainersID, textureComp);
    eManager->setComponentData<GUIComponent>(abilitiesContainersID, guiComponent);
    //Time
    timeID = eManager->createEntity();
    eManager->addComponent(timeID, ComponentType::Render);
    eManager->addComponent(timeID, ComponentType::Type);
    eManager->addComponent(timeID, ComponentType::GUI);
    trComp.position = FPair(SCREEN_CENTER.x - timeTexture.getWidth() / 2, 46);
    textureComp.texture = &timeTexture;
    eManager->setComponentData<TransformComponent>(timeID, trComp);
    eManager->setComponentData<TypeComponent>(timeID, type);
    eManager->setComponentData<RenderComponent>(timeID, textureComp);
    eManager->setComponentData<GUIComponent>(timeID, guiComponent);
    //Level
    levelID = eManager->createEntity();
    eManager->addComponent(levelID, ComponentType::Render);
    eManager->addComponent(levelID, ComponentType::Type);
    eManager->addComponent(levelID, ComponentType::GUI);
    trComp.position = FPair(SCREEN_WIDTH - levelTexture.getWidth() - 2, 4);
    textureComp.texture = &levelTexture;
    eManager->setComponentData<TransformComponent>(levelID, trComp);
    eManager->setComponentData<TypeComponent>(levelID, type);
    eManager->setComponentData<RenderComponent>(levelID, textureComp);
    eManager->setComponentData<GUIComponent>(levelID, guiComponent);
    //Pause
    pauseID = eManager->createEntity();
    eManager->addComponent(pauseID, ComponentType::Render);
    eManager->addComponent(pauseID, ComponentType::Type);
    eManager->addComponent(pauseID, ComponentType::Collision);
    eManager->addComponent(pauseID, ComponentType::GUI);
    trComp.position = FPair(SCREEN_WIDTH - pauseTexture.getWidth() - 18, 46);
    colComp.position = {trComp.position.x - hoveredOffset, trComp.position.y - hoveredOffset};
    colComp.height = pauseTexture.getHeight() + 2 * hoveredOffset;
    colComp.width = pauseTexture.getWidth() + 2 * hoveredOffset;
    textureComp.texture = &pauseTexture;
    guiComponent.clickable = true;
    guiComponent.hoverable = true;
    eManager->setComponentData<TransformComponent>(pauseID, trComp);
    eManager->setComponentData<TypeComponent>(pauseID, type);
    eManager->setComponentData<RenderComponent>(pauseID, textureComp);
    eManager->setComponentData<CollisionComponent>(pauseID, colComp);
    eManager->setComponentData<GUIComponent>(pauseID, guiComponent);
}

void PlayingScreen::destroy(EntityManager* eManager) {
    eManager->destroyEntityLater(xpContainerID);
    eManager->destroyEntityLater(currentXpID);
    eManager->destroyEntityLater(timeID);
    eManager->destroyEntityLater(abilitiesContainersID);
    eManager->destroyEntityLater(levelID);
    eManager->destroyEntityLater(pauseID);
}

void PlayingScreen::handleMouseHover(std::shared_ptr<MouseMotionMessage> msg) {
    handleHover(pauseID, msg->mousePos, nullptr);
}

void PlayingScreen::handleMouseClick(std::shared_ptr<ClickMessage> msg) {
    handleClick(pauseID, msg->mousePos, [this]() {
        onPauseClick();
    });
}

void PlayingScreen::update(EntityManager* eManager, SDL_Renderer* renderer) {
    //Time
    Uint32 newSeconds = GameStateManager::getInstance().getGameTimeSeconds();
    if (GameStateManager::getInstance().getState() == GameState::Playing && currentSeconds != newSeconds) {
        std::string timeText = formatTimeMMSS(currentSeconds);
        timeTexture.loadFromText(timeText, Colors::White, Fonts::Subtitle);
        currentSeconds = newSeconds;
    }
    //Experience
    std::ostringstream lvlText;
    auto players = eManager->getEntitiesWithComponent(ComponentType::Player);
    for (uint32_t player : players) {
        PlayerComponent* playerComp = eManager->getComponentDataPtr<PlayerComponent>(player);
        drawCurrentXp(renderer, playerComp->currentXp, playerComp->xpToNextLevel);
        if (previousLvl != playerComp->level) {
            lvlText << "lvl:" << playerComp->level;
            levelTexture.loadFromText(lvlText.str(), Colors::White, Fonts::Body);
            previousLvl = playerComp->level;
        }
    }
}

void PlayingScreen::drawAbilityContainers(SDL_Renderer* renderer) {
    abilitiesContainersTexture.createEmptyTexture(195, 35);
    SDL_SetRenderTarget(renderer, abilitiesContainersTexture.getTexture());
    SDL_SetRenderDrawColor(renderer, 230, 230, 230, 100);
    for (int i = 0; i < 5; ++i) {
        SDL_FRect rect = { i * (35 + 5), 0, 35, 35 }; // Draw 10x10 rectangles
        SDL_RenderFillRect(renderer, &rect); // Fill the rectangle
    }
    SDL_SetRenderTarget(renderer, NULL);
}

void PlayingScreen::drawXpContainer(SDL_Renderer* renderer) {
    xpContainerTexture.createEmptyTexture(SCREEN_WIDTH - 6, 40);
    SDL_SetRenderTarget(renderer, xpContainerTexture.getTexture());
    SDL_SetRenderDrawColor(renderer, 5, 5, 5, 255);
    SDL_FRect rect = {0, 0, SCREEN_WIDTH - 6, 40};
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderTarget(renderer, NULL);
}

void PlayingScreen::drawCurrentXp(SDL_Renderer* renderer, int currentXp, int xpToNextLevel) {
    if (previousXp != currentXp) {
        int maxWidth = SCREEN_WIDTH - 8;
        float currentWidth = float(currentXp) / float(xpToNextLevel) * maxWidth;
        SDL_SetRenderTarget(renderer, currentXpTexture.getTexture());
        if (currentXp < previousXp) {
            SDL_FRect rect = {0, 0, maxWidth, 38};
            SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
            SDL_RenderFillRect(renderer, &rect);
        }
        if (currentWidth > 0) {
            SDL_SetRenderDrawColor(renderer, 
                Colors::Experience.r, Colors::Experience.g, Colors::Experience.b, 255);
            SDL_FRect rect = {0, 0, currentWidth, 38};
            SDL_RenderFillRect(renderer, &rect);
        }
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_SetRenderTarget(renderer, NULL);
        previousXp = currentXp;
    }
}

void PlayingScreen::onPauseClick() {
    GameStateManager::getInstance().setState(GameState::Paused);
}
