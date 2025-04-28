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
    currentHealthTexture.m_renderer = renderer;
    healthBarTexture.m_renderer = renderer;
    drawAbilityContainers(renderer);
    drawXpContainer(renderer);
    currentXpTexture.createEmptyTexture(xpContainerTexture.getWidth() - 4, xpContainerTexture.getHeight() - 2);
    timeTexture.loadFromText("00:00", Colors::White, Fonts::Subtitle);
    levelTexture.loadFromText("lvl: 1" , Colors::White, Fonts::Body);
    pauseTexture.loadFromFile("data/img/pause.bmp");
    currentHealthTexture.createEmptyTexture(std::round(GUI::HEALTHBAR_WIDTH), std::round(GUI::HEALTHBAR_HEIGHT));
    healthBarTexture.loadFromFile("data/img/healthBar.bmp");

    TypeComponent type = EntityType::GUI;
    GUIComponent guiComponent;
    TransformComponent trComp;
    CollisionComponent colComp;
    RenderComponent textureComp;
    //XP Container
    xpContainerID = eManager->createEntity();
    EntityHandle entity = {xpContainerID, eManager};
    entity.add<RenderComponent>();
    entity.add<TypeComponent>();
    entity.add<GUIComponent>();
    trComp.position = FPair(3, 3);
    textureComp.texture = &xpContainerTexture;
    entity.set<TransformComponent>(trComp);
    entity.set<TypeComponent>(type);
    entity.set<RenderComponent>(textureComp);
    entity.set<GUIComponent>(guiComponent);
    //Current XP
    currentXpID = eManager->createEntity();
    entity.id = currentXpID;
    entity.add<RenderComponent>();
    entity.add<TypeComponent>();
    entity.add<GUIComponent>();
    trComp.position = FPair(4, 4);
    textureComp.texture = &currentXpTexture;
    entity.set<TransformComponent>(trComp);
    entity.set<TypeComponent>(type);
    entity.set<RenderComponent>(textureComp);
    entity.set<GUIComponent>(guiComponent);
    //Abilities Container
    abilitiesContainersID = eManager->createEntity();
    entity.id = abilitiesContainersID;
    entity.add<RenderComponent>();
    entity.add<TypeComponent>();
    entity.add<GUIComponent>();
    trComp.position = FPair(3, 46);
    textureComp.texture = &abilitiesContainersTexture;
    entity.set<TransformComponent>(trComp);
    entity.set<TypeComponent>(type);
    entity.set<RenderComponent>(textureComp);
    entity.set<GUIComponent>(guiComponent);
    //Time
    timeID = eManager->createEntity();
    entity.id = timeID;
    entity.add<RenderComponent>();
    entity.add<TypeComponent>();
    entity.add<GUIComponent>();
    eManager->addComponent(timeID, ComponentType::Render);
    eManager->addComponent(timeID, ComponentType::Type);
    eManager->addComponent(timeID, ComponentType::GUI);
    trComp.position = FPair(SCREEN_CENTER.x - timeTexture.getWidth() / 2, 46);
    textureComp.texture = &timeTexture;
    entity.set<TransformComponent>(trComp);
    entity.set<TypeComponent>(type);
    entity.set<RenderComponent>(textureComp);
    entity.set<GUIComponent>(guiComponent);
    //Level
    levelID = eManager->createEntity();
    entity.id = levelID;
    entity.add<RenderComponent>();
    entity.add<TypeComponent>();
    entity.add<GUIComponent>();
    trComp.position = FPair(SCREEN_WIDTH - levelTexture.getWidth() - 2, 4);
    textureComp.texture = &levelTexture;
    entity.set<TransformComponent>(trComp);
    entity.set<TypeComponent>(type);
    entity.set<RenderComponent>(textureComp);
    entity.set<GUIComponent>(guiComponent);
    //Pause
    pauseID = eManager->createEntity();
    entity.id = pauseID;
    entity.add<RenderComponent>();
    entity.add<TypeComponent>();
    entity.add<GUIComponent>();
    entity.add<CollisionComponent>();
    trComp.position = FPair(SCREEN_WIDTH - pauseTexture.getWidth() - 18, 46);
    colComp.position = {trComp.position.x - hoveredOffset, trComp.position.y - hoveredOffset};
    colComp.height = pauseTexture.getHeight() + 2 * hoveredOffset;
    colComp.width = pauseTexture.getWidth() + 2 * hoveredOffset;
    textureComp.texture = &pauseTexture;
    guiComponent.clickable = true;
    guiComponent.hoverable = true;
    entity.set<TransformComponent>(trComp);
    entity.set<TypeComponent>(type);
    entity.set<RenderComponent>(textureComp);
    entity.set<GUIComponent>(guiComponent);
    entity.set<CollisionComponent>(colComp);
    // Current health
    currentHealthID = eManager->createEntity();
    entity.id = currentHealthID;
    entity.add<RenderComponent>();
    entity.add<TypeComponent>();
    entity.add<GUIComponent>();
    trComp.position.x = SCREEN_WIDTH - GUI::HEALTHBAR_WIDTH;
    trComp.position.y += SCREEN_CENTER.y - GUI::HEALTHBAR_HEIGHT + 1;
    textureComp.texture = &currentHealthTexture;
    guiComponent.clickable = false;
    guiComponent.hoverable = false;
    entity.set<TransformComponent>(trComp);
    entity.set<TypeComponent>(type);
    entity.set<RenderComponent>(textureComp);
    entity.set<GUIComponent>(guiComponent);
    // HealthBar
    healthBarID = eManager->createEntity();
    entity.id = healthBarID;
    entity.add<RenderComponent>();
    entity.add<TypeComponent>();
    entity.add<GUIComponent>();
    trComp.position.y -= 1;
    textureComp.texture = &healthBarTexture;
    entity.set<TransformComponent>(trComp);
    entity.set<TypeComponent>(type);
    entity.set<RenderComponent>(textureComp);
    entity.set<GUIComponent>(guiComponent);
}

void PlayingScreen::destroy(EntityManager* eManager) {
    eManager->destroyEntityLater(xpContainerID);
    eManager->destroyEntityLater(currentXpID);
    eManager->destroyEntityLater(timeID);
    eManager->destroyEntityLater(abilitiesContainersID);
    eManager->destroyEntityLater(levelID);
    eManager->destroyEntityLater(pauseID);
    eManager->destroyEntityLater(healthBarID);
    eManager->destroyEntityLater(currentHealthID);
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
    //Experience, lvl & health
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
        // Health
        HealthComponent* hComp = eManager->getComponentDataPtr<HealthComponent>(player);
        drawCurrentHealth(renderer, hComp->health, hComp->maxHealth);
    }
}

void PlayingScreen::drawAbilityContainers(SDL_Renderer* renderer) {
    abilitiesContainersTexture.createEmptyTexture(195, 35);
    SDL_SetRenderTarget(renderer, abilitiesContainersTexture.getTexture());
    SDL_SetRenderDrawColor(renderer, 230, 230, 230, 100);
    for (int i = 0; i < 5; ++i) {
        SDL_FRect rect = { i * (35.0f + 5.0f), 0, 35, 35 }; // Draw 10x10 rectangles
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
        float maxWidth = SCREEN_WIDTH - 8.0f;
        float currentWidth = float(currentXp) / float(xpToNextLevel) * maxWidth;
        SDL_SetRenderTarget(renderer, currentXpTexture.getTexture());
        if (currentXp < previousXp) {
            SDL_FRect rect = {0, 0, maxWidth, 38};
            SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
            SDL_RenderFillRect(renderer, &rect);
        }
        if (currentWidth > 0) {
            SDL_SetRenderDrawColor(renderer, 
                Colors::Experience.r, Colors::Experience.g, Colors::Experience.b, Colors::Experience.a);
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

void PlayingScreen::drawCurrentHealth(SDL_Renderer* renderer, const float& currentHealth, const float& maxHealth) {
    if (previousHealth != currentHealth) {
        SDL_SetRenderTarget(renderer, currentHealthTexture.getTexture());
        float percentage = currentHealth / maxHealth;
        float height = GUI::HEALTHBAR_HEIGHT * percentage;
        float startPoint = 400.0f - height;
        SDL_FRect fullRect = {0, 0, GUI::HEALTHBAR_WIDTH, GUI::CONTAINER_HEIGHT};
        SDL_FRect rect = {0, startPoint, GUI::HEALTHBAR_WIDTH, height};
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderFillRect(renderer, &fullRect);
        SDL_SetRenderDrawColor(renderer, Colors::Health.r, Colors::Health.g, Colors::Health.b, Colors::Health.a);
        SDL_RenderFillRect(renderer, &rect);
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_SetRenderTarget(renderer, NULL);
        std::cout << "Current health: " << currentHealth << std::endl;
        std::cout << "Draw health with rect: " << rect.x << ", " << rect.y << ", " << rect.w << ", " << rect.h << std::endl;
        previousHealth = currentHealth;
    }
}
