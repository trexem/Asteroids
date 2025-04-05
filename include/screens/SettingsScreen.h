#pragma once

#include "Screen.h"
#include "texture.hpp"

class SettingsScreen : public Screen {
    SDL_Texture* playTexture = nullptr;
    SDL_Texture* settingsTexture = nullptr;
    SDL_Texture* quitTexture = nullptr;

    void handleMouseHover(std::shared_ptr<MouseMotionMessage> msg);
    void handleMouseClick(std::shared_ptr<ClickMessage> msg);

public:
    SettingsScreen(EntityManager* eManager) : Screen(eManager) {}
    void create(EntityManager* eManager, SDL_Renderer* renderer);
    void destroy(EntityManager* eManager);
};
