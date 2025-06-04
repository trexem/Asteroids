#include "EntityHandle.h"
#include "screens/Components/Button.h"
#include "texture.hpp"

//Button no 9grid
Button::Button(EntityManager& em, const std::string& label, FPair pos, FPair size,
    Texture* texture, SDL_Renderer* renderer, uint32_t parent, TTF_Font* font, SDL_Color color)
    : id(em.createEntity()), originalPos(pos), originalSize(size) {
    EntityHandle handle {id, em};

    GUIComponent guiComp {pos, size, parent};
    GUIStateComponent state;
    ButtonComponent button {label, false};
    RenderComponent render;
    TypeComponent type {EntityType::GUI};
    
    handle.add<GUIComponent>();
    handle.add<GUIStateComponent>();
    handle.add<ButtonComponent>();
    handle.set(guiComp);
    handle.set(state);
    handle.set(button);
    handle.set(type);

    if (texture != nullptr) {
        hasTexture = true;
        render.texture = texture;
        box.emplace(em, pos, size, texture);
    }

    //Button Label
    labelID = em.createEntity();
    handle.id = labelID;
    LabelComponent labelComp {label};
    labelTexture.m_renderer = renderer;
    labelTexture.loadMultilineText(label, color, font, size.x - 20);
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

//Button with 9grid
Button::Button(EntityManager& em, const std::string& label, FPair pos, FPair size,
    Texture* texture, const NineGridComponent& nineGrid, SDL_Renderer* renderer, uint32_t parent, TTF_Font* font, SDL_Color color)
    : originalPos(pos), originalSize(size) {

    GUIComponent guiComp{pos, size, parent};
    GUIStateComponent state;
    ButtonComponent button{label, false};
    TypeComponent type{EntityType::GUI};


    // Box with 9-grid
    hasTexture = true;
    box.emplace(em, pos, size, texture, true, nineGrid, parent, color);
    id = box->id;

    // Label entity
    labelID = em.createEntity();
    EntityHandle handle{labelID, em};
    LabelComponent labelComp{label};
    labelTexture.m_renderer = renderer;
    labelTexture.loadMultilineText(label, color, font, size.x - 20);

    RenderComponent render;
    render.texture = &labelTexture;
    guiComp.parent = id;

    labelSize = {labelTexture.getWidth(), labelTexture.getHeight()};
    labelPos = {size.x / 2.0f - labelSize.x / 2.0f,
                size.y / 2.0f - labelSize.y / 2.0f};
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

void Button::destroy(EntityManager& em) {
    em.destroyEntityLater(id);
    if (box) box->destroy(em);
    em.destroyEntityLater(labelID);
}

void Button::updateState(EntityManager& em) {
    GUIState state = em.getComponentDataPtr<GUIStateComponent>(id)->state;
    if (box) box->updateState(em);
    if (lastState != state) {
        setSizeFromState(em, labelID, state);
        lastState = state;
    }
}

void Button::setSizeFromState(EntityManager& em, uint32_t e, GUIState state) {
    RenderComponent render = em.getComponentData<RenderComponent>(e);
    GUIComponent gComp = em.getComponentData<GUIComponent>(e);
    switch (state) {
    case GUIState::Selected:
    case GUIState::Hovered:
        render.color = &Colors::White;
        gComp.pos = e == id ? originalPos - originalSize * 0.05f 
            : labelPos - labelSize * 0.05f;
        render.size = 1.1f;
        break;
    case GUIState::Pressed:
    case GUIState::Clicked:
        render.color = &Colors::White;
        gComp.pos = e == id ? originalPos + originalSize * 0.05f 
            : labelPos + labelSize * 0.05f;
        render.size = 0.9f;
        break;
    case GUIState::Disabled:
        render.color = &Colors::Disabled;
        break;
    case GUIState::Idle:
    default:
        gComp.pos = e == id ? originalPos : labelPos;
        render.color = &Colors::White;
        render.size = 1.0f;
        break;
    }
    em.setComponentData(e, gComp);
    em.setComponentData(e, render);
}
