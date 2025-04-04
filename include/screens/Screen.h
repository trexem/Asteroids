#pragma once

#include "EntityManager.h"
#include <SDL3/SDL.h>

class Screen {
public:
    virtual ~Screen() = default;
    virtual void create(EntityManager* eManager, SDL_Renderer* renderer) = 0;
    virtual void destroy(EntityManager* eManager) = 0;
};
