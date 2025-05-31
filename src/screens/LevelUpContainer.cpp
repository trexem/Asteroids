#include "LevelUpContainer.h"
#include "TextureManager.h"
#include "TextManager.h"

LevelUpContainer::LevelUpContainer(EntityManager& eManager, AbilityChoice choice, std::string text, FPair pos, SDL_Renderer* renderer) {
    abilityText.m_renderer = renderer;
    abilityContainer.m_renderer = renderer;
    TypeComponent type = EntityType::GUI;
    TransformComponent trComp;
    RenderComponent textureComp;
    GUIComponent guiComp;
    guiComp.pos = pos;
    guiComp.pos.y = GUI::screenHeight * 0.1851f;


    

    abilityText.loadMultilineText(text, Colors::White, Fonts::LevelUpBody, static_cast<int>(GUI::containerWidth) - 30);
    abilityIcon = TextureManager::instance().get(choice.type == AbilityType::Weapon ?
         std::string(weaponId[choice.index]) + "Icon" : std::string(passiveId[choice.index]) + "Icon");
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

    //Title
    FPair titlePos =  {5.0f, GUI::containerGap}; 
    FPair size = {GUI::containerWidth, 50.0f};
    std::string titleText = getAbilityTitle(choice);
    title = std::make_unique<Label>(eManager, titleText, titlePos, size, renderer, containerId, Fonts::LevelUpTitle);

    //Icon
    iconId = eManager.createEntity();
    eManager.addComponent(iconId, ComponentType::Render);
    eManager.addComponent(iconId, ComponentType::Type);
    eManager.addComponent(iconId, ComponentType::GUI);
    textureComp.texture = abilityIcon;
    guiComp.pos.x += GUI::containerWidth / 2 - abilityIcon->getWidth();
    guiComp.pos.y += GUI::containerGap * 2 + titlePos.y;
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
    guiComp.pos.x = pos.x + GUI::containerGap;
    guiComp.pos.y += GUI::containerGap + abilityIcon->getHeight();
    eManager.setComponentData<TransformComponent>(textId, trComp);
    eManager.setComponentData<TypeComponent>(textId, type);
    eManager.setComponentData<RenderComponent>(textId, textureComp);
    eManager.setComponentData<GUIComponent>(textId, guiComp);
}

void LevelUpContainer::destroy(EntityManager& eM) {
    title->destroy(eM);
}
