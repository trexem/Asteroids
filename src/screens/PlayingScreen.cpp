#include "ClickMessage.h"
#include "Colors.h"
#include "EntityHandle.h"
#include "Fonts.h"
#include "GameSessionManager.h"
#include "GameStateManager.h"
#include "GameStatsManager.h"
#include "GUI.h"
#include "MouseMotionMessage.h"
#include "PlayingScreen.h"
#include "texture.hpp"

PlayingScreen::~PlayingScreen() {
    GameStatsManager::instance().addCoins(GameSessionManager::instance().getStats().gold);
    GameStatsManager::instance().save("data/stats.json");
    std::cout << "PlayingScreen destroyed\n";
}

void PlayingScreen::create(EntityManager* eManager, SDL_Renderer* renderer) {
    GameStateManager::instance().startTimer();
    
    xpContainerTexture.m_renderer = renderer;
    currentXpTexture.m_renderer = renderer;
    abilitiesContainersTexture.m_renderer = renderer;
    timeTexture.m_renderer = renderer;
    levelTexture.m_renderer = renderer;
    pauseTexture.m_renderer = renderer;
    currentHealthTexture.m_renderer = renderer;
    healthBarTexture.m_renderer = renderer;
    goldTexture.m_renderer = renderer;
    enemiesTexture.m_renderer = renderer;
    drawAbilityContainers(renderer);
    drawXpContainer(renderer);
    currentXpTexture.createEmptyTexture(xpContainerTexture.getWidth() - 4, xpContainerTexture.getHeight() - 2);
    timeTexture.loadFromText("00:00", Colors::White, Fonts::Subtitle);
    levelTexture.loadFromText("lvl: 1" , Colors::White, Fonts::Body);
    pauseTexture.loadFromFile("data/img/gui/pause.bmp");
    currentHealthTexture.createEmptyTexture(std::round(GUI::healthbarWidth), std::round(GUI::healtbarHeight));
    healthBarTexture.loadFromFile("data/img/healthBar.bmp");
    goldTexture.loadFromText("gold: 0000", Colors::White, Fonts::Body);
    enemiesTexture.loadFromText("asteroids destroyed: 0000", Colors::White, Fonts::Body);

    TypeComponent type = EntityType::GUI;
    GUIComponent guiComp;
    TransformComponent trComp;
    CollisionComponent colComp;
    RenderComponent textureComp;
    //XP Container
    xpContainerID = eManager->createEntity();
    EntityHandle entity = {xpContainerID, eManager};
    entity.add<RenderComponent>();
    entity.add<TypeComponent>();
    entity.add<GUIComponent>();
    guiComp.pos = FPair(3, 3);
    textureComp.texture = &xpContainerTexture;
    entity.set<TransformComponent>(trComp);
    entity.set<TypeComponent>(type);
    entity.set<RenderComponent>(textureComp);
    entity.set<GUIComponent>(guiComp);
    //Current XP
    currentXpID = eManager->createEntity();
    entity.id = currentXpID;
    entity.add<RenderComponent>();
    entity.add<TypeComponent>();
    entity.add<GUIComponent>();
    guiComp.pos = FPair(4, 4);
    textureComp.texture = &currentXpTexture;
    entity.set<TransformComponent>(trComp);
    entity.set<TypeComponent>(type);
    entity.set<RenderComponent>(textureComp);
    entity.set<GUIComponent>(guiComp);
    //Abilities Container
    abilitiesContainersID = eManager->createEntity();
    entity.id = abilitiesContainersID;
    entity.add<RenderComponent>();
    entity.add<TypeComponent>();
    entity.add<GUIComponent>();
    guiComp.pos = FPair(3, 46);
    textureComp.texture = &abilitiesContainersTexture;
    entity.set<TransformComponent>(trComp);
    entity.set<TypeComponent>(type);
    entity.set<RenderComponent>(textureComp);
    entity.set<GUIComponent>(guiComp);
    //Time
    timeID = eManager->createEntity();
    entity.id = timeID;
    entity.add<RenderComponent>();
    entity.add<TypeComponent>();
    entity.add<GUIComponent>();
    eManager->addComponent(timeID, ComponentType::Render);
    eManager->addComponent(timeID, ComponentType::Type);
    eManager->addComponent(timeID, ComponentType::GUI);
    guiComp.pos = FPair(GUI::screenCenter.x - timeTexture.getWidth() / 2, 46);
    textureComp.texture = &timeTexture;
    entity.set<TransformComponent>(trComp);
    entity.set<TypeComponent>(type);
    entity.set<RenderComponent>(textureComp);
    entity.set<GUIComponent>(guiComp);
    //Level
    levelID = eManager->createEntity();
    entity.id = levelID;
    entity.add<RenderComponent>();
    entity.add<TypeComponent>();
    entity.add<GUIComponent>();
    guiComp.pos = FPair(GUI::screenWidth - levelTexture.getWidth() - 2, 4);
    textureComp.texture = &levelTexture;
    entity.set<TransformComponent>(trComp);
    entity.set<TypeComponent>(type);
    entity.set<RenderComponent>(textureComp);
    entity.set<GUIComponent>(guiComp);
    //Pause
    pauseID = eManager->createEntity();
    entity.id = pauseID;
    entity.add<RenderComponent>();
    entity.add<TypeComponent>();
    entity.add<GUIComponent>();
    entity.add<CollisionComponent>();
    guiComp.pos = FPair(GUI::screenWidth - pauseTexture.getWidth() - 18, 46);
    colComp.position = {guiComp.pos.x - hoveredOffset, guiComp.pos.y - hoveredOffset};
    colComp.height = pauseTexture.getHeight() + 2 * hoveredOffset;
    colComp.width = pauseTexture.getWidth() + 2 * hoveredOffset;
    textureComp.texture = &pauseTexture;
    // guiComponent.clickable = true;
    // guiComponent.hoverable = true;
    entity.set<TransformComponent>(trComp);
    entity.set<TypeComponent>(type);
    entity.set<RenderComponent>(textureComp);
    entity.set<GUIComponent>(guiComp);
    entity.set<CollisionComponent>(colComp);
    // Current Gold
    goldID = eManager->createEntity();
    entity.id = goldID;
    entity.add<RenderComponent>();
    entity.add<TypeComponent>();
    entity.add<GUIComponent>();
    guiComp.pos.x -= goldTexture.getWidth() - 10.0f;
    textureComp.texture = &goldTexture;
    entity.set<TransformComponent>(trComp);
    entity.set<TypeComponent>(type);
    entity.set<RenderComponent>(textureComp);
    entity.set<GUIComponent>(guiComp);
    // Current enemies
    enemiesID = eManager->createEntity();
    entity.id = enemiesID;
    entity.add<RenderComponent>();
    entity.add<TypeComponent>();
    entity.add<GUIComponent>();
    guiComp.pos.x -= enemiesTexture.getWidth() - 10.0f;
    textureComp.texture = &enemiesTexture;
    entity.set<TransformComponent>(trComp);
    entity.set<TypeComponent>(type);
    entity.set<RenderComponent>(textureComp);
    entity.set<GUIComponent>(guiComp);
    // Current health
    currentHealthID = eManager->createEntity();
    entity.id = currentHealthID;
    entity.add<RenderComponent>();
    entity.add<TypeComponent>();
    entity.add<GUIComponent>();
    guiComp.pos.x = GUI::screenWidth - GUI::healthbarWidth;
    guiComp.pos.y = GUI::screenCenter.y - GUI::healtbarHeight / 2 + 1;
    textureComp.texture = &currentHealthTexture;
    // guiComponent.clickable = false;
    // guiComponent.hoverable = false;
    entity.set<TransformComponent>(trComp);
    entity.set<TypeComponent>(type);
    entity.set<RenderComponent>(textureComp);
    entity.set<GUIComponent>(guiComp);
    // HealthBar
    healthBarID = eManager->createEntity();
    entity.id = healthBarID;
    entity.add<RenderComponent>();
    entity.add<TypeComponent>();
    entity.add<GUIComponent>();
    guiComp.pos.y -= 1;
    textureComp.texture = &healthBarTexture;
    entity.set<TransformComponent>(trComp);
    entity.set<TypeComponent>(type);
    entity.set<RenderComponent>(textureComp);
    entity.set<GUIComponent>(guiComp);
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
    Uint32 newSeconds = GameStateManager::instance().getGameTimeSeconds();
    if (GameStateManager::instance().getState() == GameState::Playing && currentSeconds != newSeconds) {
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
            previousLvl = playerComp->level;
            lvlText << "lvl:" << previousLvl;
            levelTexture.loadFromText(lvlText.str(), Colors::White, Fonts::Body);
        }
        // Health
        HealthComponent* hComp = eManager->getComponentDataPtr<HealthComponent>(player);
        drawCurrentHealth(renderer, hComp->health, hComp->maxHealth);
    }
    // Gold
    if (GameSessionManager::instance().getStats().gold != lastGold) {
        lastGold = GameSessionManager::instance().getStats().gold;
        std::ostringstream goldText;
        goldText << "gold: " << lastGold;
        goldTexture.loadFromText(goldText.str(), Colors::White, Fonts::Body);
    }
    // Enemies
    if (GameSessionManager::instance().getStats().asteroidsDestroyed != lastEnemies) {
        lastEnemies = GameSessionManager::instance().getStats().asteroidsDestroyed;
        std::ostringstream enemiesText;
        enemiesText << "asteroids destroyed: " << lastEnemies;
        enemiesTexture.loadFromText(enemiesText.str(), Colors::White, Fonts::Body);
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
    xpContainerTexture.createEmptyTexture(GUI::screenWidth - 6, 40);
    SDL_SetRenderTarget(renderer, xpContainerTexture.getTexture());
    SDL_SetRenderDrawColor(renderer, 5, 5, 5, 255);
    SDL_FRect rect = {0, 0, GUI::screenWidth - 6, 40};
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderTarget(renderer, NULL);
}

void PlayingScreen::drawCurrentXp(SDL_Renderer* renderer, int currentXp, int xpToNextLevel) {
    if (previousXp != currentXp) {
        float maxWidth = GUI::screenWidth - 8.0f;
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
    GameStateManager::instance().setState(GameState::Paused);
}

void PlayingScreen::drawCurrentHealth(SDL_Renderer* renderer, const float& currentHealth, const float& maxHealth) {
    if (previousHealth != currentHealth) {
        SDL_SetRenderTarget(renderer, currentHealthTexture.getTexture());
        float percentage = currentHealth / maxHealth;
        float height = GUI::healtbarHeight * percentage;
        float startPoint = 400.0f - height;
        SDL_FRect fullRect = {0, 0, GUI::healthbarWidth, GUI::containerHeight};
        SDL_FRect rect = {0, startPoint, GUI::healthbarWidth, height};
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderFillRect(renderer, &fullRect);
        SDL_SetRenderDrawColor(renderer, Colors::Health.r, Colors::Health.g, Colors::Health.b, Colors::Health.a);
        SDL_RenderFillRect(renderer, &rect);
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_SetRenderTarget(renderer, NULL);
        // std::cout << "Current health: " << currentHealth << std::endl;
        // std::cout << "Draw health with rect: " << rect.x << ", " << rect.y << ", " << rect.w << ", " << rect.h << std::endl;
        previousHealth = currentHealth;
    }
}
