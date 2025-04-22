#include "RenderSystem.h"

RenderSystem::RenderSystem(SDL_Window* window, const char * name, Camera* camera)
    : renderer(std::make_unique<Renderer>(window, name)), camera(camera) {
    renderer->changeColor(0x00, 0x00, 0x00, 0xFF);
}

SDL_Renderer* RenderSystem::getRenderer() {
	return renderer->getRenderer();
}

void RenderSystem::render(EntityManager& eM) {
    renderer->clear();
    const SDL_Color debugColor = Colors::Experience;
    // std::cout << "Rendering frame: " << frame << std::endl;
    for (uint32_t eID : eM.getEntitiesWithComponent(ComponentType::Render)) {
        if (eM.isMarkedForDestruction(eID)) continue;
        if (! eM.hasComponent<GUIComponent>(eID)) {
            RenderComponent rComp = eM.getComponentData<RenderComponent>(eID);
            TransformComponent trComp = eM.getComponentData<TransformComponent>(eID);
            FPair position;
            position.x = trComp.position.x - camera->position.x;
            position.y = trComp.position.y - camera->position.y;
            const int w = rComp.isStretched ? rComp.exactSize.x : rComp.texture->getWidth();
            const int h = rComp.isStretched ? rComp.exactSize.y : rComp.texture->getHeight();
            SDL_FPoint pivot = getPivotFromRotationPoint(
                rComp.texture->rotationPoint, w, h);
            if (rComp.isStretched) {
                rComp.texture->renderEx(
                    static_cast<int>(position.x),
                    static_cast<int>(position.y),
                    nullptr,
                    static_cast<int>(trComp.rotDegrees),
                    &pivot,
                    SDL_FLIP_NONE,
                    rComp.exactSize
                );
            } else {
                rComp.texture->renderEx(
                    static_cast<int>(position.x),
                    static_cast<int>(position.y),
                    nullptr,
                    static_cast<int>(trComp.rotDegrees),
                    &pivot,
                    SDL_FLIP_NONE,
                    rComp.size
                );
            }
            SDL_FRect debugRect = { position.x, position.y, w, h};
            renderer->drawDebugRect(debugRect, debugColor);
        }
    }
    for (uint32_t eID : eM.getEntitiesWithComponent(ComponentType::GUI)) {
        RenderComponent rComp = eM.getComponentData<RenderComponent>(eID);
        TransformComponent trComp = eM.getComponentData<TransformComponent>(eID);
        rComp.texture->renderEx(
            static_cast<int>(trComp.position.x),
            static_cast<int>(trComp.position.y),
            nullptr,
            static_cast<int>(trComp.rotDegrees),
            nullptr,
            SDL_FLIP_NONE,
            rComp.size
        );
    }
    renderer->render();
    frame++;
    saveRendererToImage();
}


//Used for debugging purposes but might be useful in the future to save actual screenshots (?)
void RenderSystem::saveRendererToImage() {
    int width, height;
    SDL_GetCurrentRenderOutputSize(renderer->getRenderer(), &width, &height);
    SDL_Surface* surface = SDL_RenderReadPixels(renderer->getRenderer(), NULL);
    // Save the surface to a BMP file (you can use SDL_SaveBMP or SDL_Image for PNG, JPG, etc.)
    if (!SDL_SaveBMP(surface, "screenshot.bmp")) {
        std::cerr << "SDL_SaveBMP failed: " << SDL_GetError() << std::endl;
    } else {
        std::cout << "Saved screenshot as screenshot.bmp" << std::endl;
    }

    // Clean up the surface
    SDL_DestroySurface(surface);
}
