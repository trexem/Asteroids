#pragma once

#include "Camera.h"
#include "System.h"
#include "renderer.hpp"
#include "Colors.h"

#include <SDL3_image/SDL_image.h>

class RenderSystem : public System {
public:
    RenderSystem(SDL_Window* window, const char * name, Camera* camera);
    ~RenderSystem() = default;
    void render(EntityManager& eM);
    SDL_Renderer* getRenderer();
    SDL_Texture* screenTexture;
    void saveRendererToImage();
    void drawGameEntities(EntityManager& eM);
    void drawGUI(EntityManager& eM);
private:
    std::unique_ptr<Renderer> renderer;
    Camera* camera;
    uint32_t frame {0};
};
