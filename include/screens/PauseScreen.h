#pragma once

#include "Screen.h"
#include "texture.hpp"

class PauseScreen : public Screen {
    Texture playTexture = nullptr;
    Texture settingsTexture = nullptr;
    Texture quitTexture = nullptr;

    

    void handleMouseHover(std::shared_ptr<MouseMotionMessage> msg);
    void handleMouseClick(std::shared_ptr<ClickMessage> msg);

public:
    PauseScreen(EntityManager* eManager) : Screen(eManager) {}
    void create(EntityManager* eManager, SDL_Renderer* renderer);
    void destroy(EntityManager* eManager);
    void update(EntityManager* eManager, SDL_Renderer* renderer);
};
