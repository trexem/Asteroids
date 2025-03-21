#pragma once
#ifndef __RENDER_SYSTEM_H_
#define __RENDER_SYSTEM_H_

#include "System.h"
#include "renderer.hpp"

class RenderSystem : public System {
public:
    RenderSystem(SDL_Window* window, const char * name);
    void render(EntityManager& eM);
    SDL_Renderer* getRenderer();
private:
    Renderer* renderer;
};

#endif // !__RENDER_SYSTEM_H_