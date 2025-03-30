#pragma once
#ifndef __RENDER_SYSTEM_H_
#define __RENDER_SYSTEM_H_

#include "Camera.h"
#include "System.h"
#include "renderer.hpp"

#include <SDL3_image/SDL_image.h>

class RenderSystem : public System {
public:
    RenderSystem(SDL_Window* window, const char * name, Camera* camera);
    ~RenderSystem() = default;
    void render(EntityManager& eM);
    SDL_Renderer* getRenderer();
    void saveRendererToImage();
private:
    std::unique_ptr<Renderer> renderer;
    Camera* camera;
    uint32_t frame {0};
};

#endif // !__RENDER_SYSTEM_H_
