#include "EntityHandle.h"
#include "screens/Components/Box.h"

Box::Box(EntityManager& eM, FPair pos, FPair size, Texture* texture, bool use9Grid,
        NineGridComponent nineGrid, uint32_t parent, SDL_Color color
) {
    id = eM.createEntity();
    EntityHandle handle {id, eM};

    GUIComponent guiComp {pos, size, parent};
    originalPos = pos;
    originalSize = size;
    GUIStateComponent state;
    RenderComponent render;
    TypeComponent type {EntityType::GUI};

    handle.add<GUIComponent>();
    handle.add<GUIStateComponent>();
    if (texture) {
        render.texture = texture;
        if (use9Grid) {
            render.exactSize = size;
            handle.add<NineGridComponent>();
            handle.set(nineGrid);
        }
        handle.add<RenderComponent>();
        handle.set(render);
    }
    
    handle.set(guiComp);
    handle.set(state);
    handle.set(type);
}

void Box::destroy(EntityManager& eM) {
    eM.destroyEntityLater(id);
}

void Box::updateState(EntityManager& em) {
    GUIState state = em.getComponentDataPtr<GUIStateComponent>(id)->state;
    if (lastState != state) {
        setSizeFromState(em, state);
        lastState = state;
    }
}

void Box::setSizeFromState(EntityManager& em, GUIState state) {
    RenderComponent* render = em.getComponentDataPtr<RenderComponent>(id);
    GUIComponent* gComp = em.getComponentDataPtr<GUIComponent>(id);
    switch (state) {
    case GUIState::Selected:
    case GUIState::Hovered:
        render->color = &Colors::White;
        gComp->pos = originalPos - originalSize * 0.05f;
        render->size = 1.1f;
        break;
    case GUIState::Pressed:
    case GUIState::Clicked:
        render->color = &Colors::White;
        gComp->pos = originalPos + originalSize * 0.05f;
        render->size = 0.9f;
        break;
    case GUIState::Disabled:
        render->color = &Colors::Disabled;
        break;
    case GUIState::Idle:
    default:
        gComp->pos = originalPos;
        render->color = &Colors::White;
        render->size = 1.0f;
        break;
    }
}
