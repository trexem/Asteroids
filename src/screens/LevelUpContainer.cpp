#include "LevelUpContainer.h"
#include "TextureManager.h"

LevelUpContainer::LevelUpContainer(EntityManager& eManager, AbilityChoice choice, std::string text, FPair pos, SDL_Renderer* renderer) {
    abilityText.m_renderer = renderer;
    abilityContainer.m_renderer = renderer;
    TypeComponent type = EntityType::GUI;
    TransformComponent trComp;
    RenderComponent textureComp;
    GUIComponent guiComp;
    guiComp.pos = pos;
    guiComp.pos.y = GUI::screenHeight * 0.1851f;
    

    abilityText.loadFromText(text, Colors::White, Fonts::Body);
    abilityIcon = TextureManager::instance().get(choice.type == AbilityType::Weapon ?
         weaponIconId[choice.index] : passiveIconId[choice.index]);
    abilityContainer.createEmptyTexture(GUI::containerWidth, GUI::containerHeight);
    SDL_SetRenderTarget(renderer, abilityContainer.getTexture());
    SDL_SetRenderDrawColor(renderer, Colors::Black.r, Colors::Black.g, Colors::Black.b, Colors::Black.a);
    SDL_FRect rect = {0, 0, GUI::containerWidth, GUI::containerHeight};
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderTarget(renderer, NULL);
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);

    //Container
    containerId = eManager.createEntity();
    eManager.addComponent(containerId, ComponentType::Render);
    eManager.addComponent(containerId, ComponentType::Type);
    eManager.addComponent(containerId, ComponentType::GUI);
    textureComp.texture = &abilityContainer;
    eManager.setComponentData<TransformComponent>(containerId, trComp);
    eManager.setComponentData<TypeComponent>(containerId, type);
    eManager.setComponentData<RenderComponent>(containerId, textureComp);
    eManager.setComponentData<GUIComponent>(containerId, guiComp);

    //Icon
    iconId = eManager.createEntity();
    eManager.addComponent(iconId, ComponentType::Render);
    eManager.addComponent(iconId, ComponentType::Type);
    eManager.addComponent(iconId, ComponentType::GUI);
    textureComp.texture = abilityIcon;
    guiComp.pos.x += GUI::containerWidth / 2 - abilityIcon->getWidth();
    guiComp.pos.y += GUI::containerGap;
    eManager.setComponentData<TransformComponent>(iconId, trComp);
    eManager.setComponentData<TypeComponent>(iconId, type);
    eManager.setComponentData<RenderComponent>(iconId, textureComp);
    eManager.setComponentData<GUIComponent>(iconId, guiComp);

    //Text
    textId = eManager.createEntity();
    eManager.addComponent(textId, ComponentType::Render);
    eManager.addComponent(textId, ComponentType::Type);
    eManager.addComponent(textId, ComponentType::GUI);
    textureComp.texture = &abilityText;
    guiComp.pos.y += GUI::containerGap + abilityIcon->getHeight();
    eManager.setComponentData<TransformComponent>(textId, trComp);
    eManager.setComponentData<TypeComponent>(textId, type);
    eManager.setComponentData<RenderComponent>(textId, textureComp);
    eManager.setComponentData<GUIComponent>(textId, guiComp);
}
