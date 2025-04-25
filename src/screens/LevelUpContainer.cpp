#include "LevelUpContainer.h"

LevelUpContainer::LevelUpContainer(EntityManager* eManager, AbilityChoice choice, std::string text, FPair pos, SDL_Renderer* renderer) {
    abilityIcon.m_renderer = renderer;
    abilityText.m_renderer = renderer;
    abilityContainer.m_renderer = renderer;
    TypeComponent type = EntityType::GUI;
    TransformComponent trComp;
    RenderComponent textureComp;
    GUIComponent guiComp;
    trComp.position = pos;
    trComp.position.y = SCREEN_HEIGHT * 0.1851f;
    

    abilityText.loadFromText(text, Colors::White, Fonts::Body);
    abilityIcon.loadFromFile(choice.type == AbilityType::Weapon ?
         weaponIconLocation[choice.index] : passiveIconLocation[choice.index]);
    abilityContainer.createEmptyTexture(GUI::CONTAINER_WIDTH, GUI::CONTAINER_HEIGHT);
    SDL_SetRenderTarget(renderer, abilityContainer.getTexture());
    SDL_SetRenderDrawColor(renderer, Colors::Black.r, Colors::Black.g, Colors::Black.b, Colors::Black.a);
    SDL_FRect rect = {0, 0, GUI::CONTAINER_WIDTH, GUI::CONTAINER_HEIGHT};
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderTarget(renderer, NULL);
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);

    //Container
    containerId = eManager->createEntity();
    eManager->addComponent(containerId, ComponentType::Render);
    eManager->addComponent(containerId, ComponentType::Type);
    eManager->addComponent(containerId, ComponentType::GUI);
    textureComp.texture = &abilityContainer;
    eManager->setComponentData<TransformComponent>(containerId, trComp);
    eManager->setComponentData<TypeComponent>(containerId, type);
    eManager->setComponentData<RenderComponent>(containerId, textureComp);
    eManager->setComponentData<GUIComponent>(containerId, guiComp);

    //Icon
    iconId = eManager->createEntity();
    eManager->addComponent(iconId, ComponentType::Render);
    eManager->addComponent(iconId, ComponentType::Type);
    eManager->addComponent(iconId, ComponentType::GUI);
    textureComp.texture = &abilityIcon;
    trComp.position.x += GUI::CONTAINER_WIDTH / 2 - abilityIcon.getWidth();
    trComp.position.y += GUI::CONTAINER_GAP;
    eManager->setComponentData<TransformComponent>(iconId, trComp);
    eManager->setComponentData<TypeComponent>(iconId, type);
    eManager->setComponentData<RenderComponent>(iconId, textureComp);
    eManager->setComponentData<GUIComponent>(iconId, guiComp);

    //Text
    textId = eManager->createEntity();
    eManager->addComponent(textId, ComponentType::Render);
    eManager->addComponent(textId, ComponentType::Type);
    eManager->addComponent(textId, ComponentType::GUI);
    textureComp.texture = &abilityText;
    trComp.position.y += GUI::CONTAINER_GAP + abilityIcon.getHeight();
    eManager->setComponentData<TransformComponent>(textId, trComp);
    eManager->setComponentData<TypeComponent>(textId, type);
    eManager->setComponentData<RenderComponent>(textId, textureComp);
    eManager->setComponentData<GUIComponent>(textId, guiComp);
}
