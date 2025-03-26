#pragma once
#ifndef __RENDER_SYSTEM_H_
#define __RENDER_SYSTEM_H_

#include "System.h"
#include "renderer.hpp"

class RenderSystem : public System {
public:
    RenderSystem(SDL_Window* window, const char * name);
    ~RenderSystem() = default;
    void render(EntityManager& eM);
    SDL_Renderer* getRenderer();
private:
    std::unique_ptr<Renderer> renderer;
};

#endif // !__RENDER_SYSTEM_H_