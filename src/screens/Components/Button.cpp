#include "EntityHandle.h"
#include "screens/Components/Button.h"
#include "texture.hpp"

Button::Button(EntityManager& em, const std::string& label, FPair pos, FPair size,
    Texture* texture, SDL_Renderer* renderer, uint32_t parent, TTF_Font* font, SDL_Color color) {
    // Button container (?)
    id = em.createEntity();
    EntityHandle handle {id, em};

    GUIComponent guiComp {pos, size, parent};
    originalPos = pos;
    originalSize = size;
    GUIStateComponent state;
    ButtonComponent button {label, false};
    RenderComponent render;
    TypeComponent type {EntityType::GUI};
    
    handle.add<GUIComponent>();
    handle.add<GUIStateComponent>();
    handle.add<ButtonComponent>();
    if (texture != nullptr) {
        hasTexture = true;
        render.texture = texture;
        handle.add<RenderComponent>();
        handle.set(render);
    }

    handle.set(guiComp);
    handle.set(state);
    handle.set(button);
    handle.set(type);
    //Button Label
    labelID = em.createEntity();
    handle.id = labelID;
    LabelComponent labelComp {label};
    labelTexture.m_renderer = renderer;
    labelTexture.loadFromText(label, color, font);
    render.texture = &labelTexture;
    guiComp.parent = id;
    labelPos = {size.x / 2.0f - labelTexture.getWidth() / 2.0f,
                   size.y / 2.0f - labelTexture.getHeight() / 2.0f};
    labelSize = {labelTexture.getWidth(), labelTexture.getHeight()};
    guiComp.pos = labelPos;
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

bool Button::wasClicked(EntityManager& em) {
    return em.getComponentDataPtr<GUIStateComponent>(id)->state == GUIState::Clicked;
}

void Button::destroy(EntityManager& em) {
    em.destroyEntityLater(id);
    em.destroyEntityLater(labelID);
}

void Button::updateState(EntityManager& em) {
    GUIState state = em.getComponentDataPtr<GUIStateComponent>(id)->state;
    if (lastState != state) {
        if (hasTexture) setSizeFromState(em, id, state);
        setSizeFromState(em, labelID, state);
        lastState = state;
    }
}

void Button::setSizeFromState(EntityManager& em, uint32_t e, GUIState state) {
    RenderComponent render = em.getComponentData<RenderComponent>(e);
    GUIComponent gComp = em.getComponentData<GUIComponent>(e);
    switch (state) {
    case GUIState::Hovered:
        gComp.pos -= e == id ? originalSize * 0.05f : labelSize * 0.05f;
        render.size = 1.1f;
        break;
    case GUIState::Pressed:
    case GUIState::Clicked:
        gComp.pos = e == id ? originalPos + originalSize * 0.05f 
            : labelPos + labelSize * 0.05f;
        render.size = 0.9f;
        break;
    case GUIState::Disabled:
        render.texture->colorMod(Colors::Disabled);
        break;
    case GUIState::Idle:
    default:
        gComp.pos = e == id ? originalPos : labelPos;
        render.texture->colorMod(Colors::White);
        render.size = 1.0f;
        break;
    }
    em.setComponentData(e, gComp);
    em.setComponentData(e, render);
}
