#include "EntityHandle.h"
#include "screens/Components/Label.h"
#include "texture.hpp"

Label::Label(EntityManager& em, const std::string& label, FPair pos, FPair size,
        SDL_Renderer* renderer, uint32_t parent, TTF_Font* font, SDL_Color color,
        bool interactive) : text(label), originalPos(pos), originalSize(size),
        font(font), color(color), interactive(interactive) {
    id = em.createEntity();
    EntityHandle handle {id, em};

    GUIComponent guiComp {pos, size, parent};
    GUIStateComponent state;
    LabelComponent labelComp {text};
    RenderComponent render;
    TypeComponent type {EntityType::GUI};

    handle.add<GUIComponent>();
    handle.add<GUIStateComponent>();
    handle.add<LabelComponent>();
    handle.add<RenderComponent>();
    texture.m_renderer = renderer;
    texture.loadFromText(text, color, font);
    render.texture = &texture;

    handle.set(guiComp);
    handle.set(state);
    handle.set(labelComp);
    handle.set(render);
    handle.set(type);
}

void Label::destroy(EntityManager& em) {
    em.destroyEntityLater(id);
}

void Label::setText(const std::string& t) {
    text = t;
    texture.loadFromText(text, color, font);
}
