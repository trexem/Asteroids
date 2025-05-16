#pragma once

#include "Colors.h"
#include "Fonts.h"
#include "Screen.h"
#include "texture.hpp"

class GameOverScreen : public Screen {
    Texture restartTexture = nullptr;
    Texture mainMenuTexture = nullptr;
    Texture quitTexture = nullptr;
    uint32_t restartID, mmID, quitID;

    void handleMouseHover(std::shared_ptr<MouseMotionMessage> msg);
    void handleMouseClick(std::shared_ptr<ClickMessage> msg);

public:
    GameOverScreen(EntityManager& eManager) : Screen(eManager) {}
    void create(EntityManager& eManager, SDL_Renderer* renderer);
    void destroy(EntityManager& eManager);
    void update(EntityManager& eManager, SDL_Renderer* renderer);
};
