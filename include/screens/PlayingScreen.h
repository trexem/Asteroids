#pragma once

#include "Screen.h"

class Texture;

class PlayingScreen : public Screen {
    Texture xpContainerTexture;
    Texture currentXpTexture;
    Texture abilitiesContainersTexture;
    Texture timeTexture;
    Texture levelTexture;
    Texture pauseTexture;
    Texture healthBarTexture;
    Texture currentHealthTexture;
    Texture goldTexture;
    Texture enemiesTexture;
    uint32_t xpContainerID, currentXpID, abilitiesContainersID;
    uint32_t timeID, levelID, pauseID, healthBarID, currentHealthID;
    uint32_t goldID, enemiesID, lastGold, lastEnemies; 
    Uint32 currentSeconds{0};
    int previousXp, previousLvl;
    float previousHealth;

    void handleMouseHover(std::shared_ptr<MouseMotionMessage> msg);
    void handleMouseClick(std::shared_ptr<ClickMessage> msg);
    void onPauseClick();
    void drawAbilityContainers(SDL_Renderer* renderer);
    void drawXpContainer(SDL_Renderer* renderer);
    void drawCurrentXp(SDL_Renderer* renderer, int currentXp, int xpToNextLevel);
    void drawCurrentHealth(SDL_Renderer* renderer, const float& currentHealth, const float& maxHealth);

public:
    PlayingScreen(EntityManager* eManager) : Screen(eManager) {
        // std::cout << "Creating PlayingScreen\n";
    }
    ~PlayingScreen();
    void create(EntityManager* eManager, SDL_Renderer* renderer);
    void destroy(EntityManager* eManager);
    void update(EntityManager* eManager, SDL_Renderer* renderer);
};
