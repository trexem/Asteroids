#pragma once

#include "EntityHandle.h"
#include "System.h"

class BackgroundSystem : public System {
public:
    BackgroundSystem(EntityManager& eManager, SDL_Renderer* renderer);
    void update(EntityManager& eManager);
    void createBackground(EntityManager& eManager);
private:
    FPair lastPlayerPos;
    Texture starTexture, galaxyTexture, nebulaTexture;
};
