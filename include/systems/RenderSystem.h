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
    void render(EntityManager& eMgr);
    SDL_Renderer* getRenderer();
    SDL_Texture* screenTexture;
    void saveRendererToImage();
    void drawGameEntities(EntityManager& eM);
    void drawGUI(EntityManager& eM);
    void update(EntityManager& eMgr, const double& dT) override {}
private:
    std::shared_ptr<Renderer> renderer;
    Camera* camera;
    uint32_t frame {0};
};
