#pragma once

#include "Screen.h"
#include "texture.hpp"

class MainMenuScreen : public Screen {
    SDL_Texture* playTexture = nullptr;
    SDL_Texture* settingsTexture = nullptr;
    SDL_Texture* quitTexture = nullptr;

public:
    void create(EntityManager* eManager, SDL_Renderer* renderer);
    void destroy(EntityManager* eManager);
};
