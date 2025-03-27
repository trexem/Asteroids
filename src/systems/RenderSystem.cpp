#include "RenderSystem.h"

RenderSystem::RenderSystem(SDL_Window* window, const char * name)
    : renderer(std::make_unique<Renderer>(window, name)) {
    renderer->changeColor(0x00, 0x00, 0x00, 0xFF);
}

SDL_Renderer* RenderSystem::getRenderer() {
	return renderer->getRenderer();
}

void RenderSystem::render(EntityManager& eM) {
    renderer->clear();
    std::cout << "Rendering frame: " << frame << std::endl;
    for (uint32_t eID : eM.getEntitiesWithComponent(ComponentType::Render)) {
        RenderComponent rComp = eM.getComponentData<RenderComponent>(eID);
        TransformComponent trComp = eM.getComponentData<TransformComponent>(eID);
        rComp.texture->renderEx(static_cast<int>(trComp.position.x),
            static_cast<int>(trComp.position.y),
            nullptr,
            static_cast<int>(trComp.rotDegrees),
            nullptr,
            SDL_FLIP_NONE
        );
    }
    std::cout << std::endl;
    renderer->render();
    frame++;
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
