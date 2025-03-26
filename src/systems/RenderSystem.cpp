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
    for (uint32_t eID : eM.getEntitiesWithComponent(ComponentType::Render)) {
        RenderComponent rComp = eM.getComponentData<RenderComponent>(eID);
        TransformComponent trComp = eM.getComponentData<TransformComponent>(eID);
        rComp.texture->renderEx((int)trComp.position.x, (int)trComp.position.y, nullptr, (int)trComp.rotDegrees, nullptr, SDL_FLIP_NONE);
    }
    renderer->render();

}