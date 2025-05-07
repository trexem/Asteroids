#include "Colors.h"
#include "EntityHandle.h"
#include "Fonts.h"
#include "screens/Components/Button.h"
#include "texture.hpp"

Button::Button(EntityManager* em, const std::string& label, FPair pos, FPair size,
    Texture* texture, SDL_Renderer* renderer, uint32_t parent) {
    // Button container (?)
    id = em->createEntity();
    EntityHandle handle {id, em};

    GUIComponent guiComp {pos, size, parent};
    GUIStateComponent state;
    ButtonComponent button {label, false};
    RenderComponent render;
    render.texture = texture;
    TypeComponent type {EntityType::GUI};

    handle.add<GUIComponent>();
    handle.add<GUIStateComponent>();
    handle.add<ButtonComponent>();
    handle.add<RenderComponent>();

    handle.set(guiComp);
    handle.set(state);
    handle.set(button);
    handle.set(render);
    handle.set(type);
    //Button Label
    labelID = em->createEntity();
    handle.id = labelID;
    LabelComponent labelComp {label};
    labelTexture.m_renderer = renderer;
    labelTexture.loadFromText(label, Colors::White, Fonts::Body);
    render.texture = &labelTexture;

    handle.add<GUIComponent>();
    handle.add<GUIStateComponent>();
    handle.add<LabelComponent>();
    handle.add<RenderComponent>();
    handle.set(guiComp);
    handle.set(state);
    handle.set(labelComp);
    handle.set(render);
    handle.set(type);
}

bool Button::wasClicked(EntityManager* em) {
    return em->getComponentDataPtr<GUIStateComponent>(id)->state == GUIState::Clicked;
}

void Button::destroy(EntityManager* em) {
    em->destroyEntityLater(id);
    em->destroyEntityLater(labelID);
}
