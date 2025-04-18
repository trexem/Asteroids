#pragma once

#include "Colors.h"
#include "Fonts.h"
#include "Screen.h"
#include "texture.hpp"
#include "utils.hpp"

class PlayingScreen : public Screen {
    Texture xpContainerTexture;
    Texture currentXpTexture;
    Texture abilitiesContainersTexture;
    Texture timeTexture;
    Texture levelTexture;
    Texture pauseTexture;
    uint32_t xpContainerID, currentXpID, abilitiesContainersID;
    uint32_t timeID, levelID, pauseID;
    Uint32 currentSeconds{0};
    int previousXp, previousLvl;

    void handleMouseHover(std::shared_ptr<MouseMotionMessage> msg);
    void handleMouseClick(std::shared_ptr<ClickMessage> msg);
    void onPauseClick();
    void drawAbilityContainers(SDL_Renderer* renderer);
    void drawXpContainer(SDL_Renderer* renderer);
    void drawCurrentXp(SDL_Renderer* renderer, int currentXp, int xpToNextLevel);

public:
    PlayingScreen(EntityManager* eManager) : Screen(eManager) {
        // std::cout << "Creating PlayingScreen\n";
    }
    ~PlayingScreen();
    void create(EntityManager* eManager, SDL_Renderer* renderer);
    void destroy(EntityManager* eManager);
    void update(EntityManager* eManager, SDL_Renderer* renderer);
};
