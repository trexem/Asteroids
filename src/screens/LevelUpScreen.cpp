#include "LevelUpScreen.h"

void LevelUpScreen::create(EntityManager* eManager, SDL_Renderer* renderer) {
    eManager = eManager;
    TypeComponent type = EntityType::GUI;
    TransformComponent trComp;
    RenderComponent textureComp;
    CollisionComponent colComp;
    GUIComponent guiComp;
    FPair pos;
    pos.y = SCREEN_HEIGHT * 0.1851f;
    container1.m_renderer = renderer;
    container2.m_renderer = renderer;
    container3.m_renderer = renderer;
    container1.createEmptyTexture(GUI::CONTAINER_WIDTH + 4, GUI::CONTAINER_HEIGHT + 4);
    container2.createEmptyTexture(GUI::CONTAINER_WIDTH + 4, GUI::CONTAINER_HEIGHT + 4);
    container3.createEmptyTexture(GUI::CONTAINER_WIDTH + 4, GUI::CONTAINER_HEIGHT + 4);
    drawConatiner(renderer, container1.getTexture());
    drawConatiner(renderer, container2.getTexture());
    drawConatiner(renderer, container3.getTexture());
    id1 = eManager->createEntity();
    eManager->addComponent(id1, ComponentType::Type);
    id2 = eManager->createEntity();
    eManager->addComponent(id2, ComponentType::Type);
    id3 = eManager->createEntity();
    eManager->addComponent(id3, ComponentType::Type);
    auto players = eManager->getEntitiesWithComponent(ComponentType::Player);
    for (uint32_t player : players) {
        PlayerComponent playerComp = eManager->getComponentData<PlayerComponent>(player);
        options = getRandomAbilityChoices(playerComp);
        if (options.size() == 0) {
            options.push_back({AbilityType::Money, static_cast<size_t>(PassiveAbilities::PassiveAbilitiesCount)});
            options.push_back({AbilityType::Health, static_cast<size_t>(PassiveAbilities::PassiveAbilitiesCount) + 1});
        }
        size_t count = options.size();
        for (int i = 0; i < count; i++) {
            if (count == 3) {
                pos.x = SCREEN_WIDTH * .15625f + i * (GUI::CONTAINER_WIDTH + GUI::CONTAINER_GAP);
            } else {
                pos.x = i == 0 ?
                SCREEN_CENTER.x - GUI::CONTAINER_WIDTH - GUI::CONTAINER_GAP :
                SCREEN_CENTER.x + GUI::CONTAINER_GAP;
            }
            //Container
            uint32_t id = i == 0 ? id1 : i == 1 ? id2 : id3;
            eManager->addComponent(id, ComponentType::Render);
            eManager->addComponent(id, ComponentType::Collision);
            eManager->addComponent(id, ComponentType::GUI);
            trComp.position = pos - 2;
            // std::cout << "containerId: " << id << " with pos: " << trComp.position.x <<
            // ", " << trComp.position.y << std::endl;
            colComp.position = trComp.position - hoveredOffset;
            colComp.height = container1.getHeight() + 2 * hoveredOffset;
            colComp.width = container1.getWidth() + 2 * hoveredOffset;
            textureComp.texture = i == 0 ? &container1 : i == 1 ? &container2 : &container3;
            eManager->setComponentData<TransformComponent>(id, trComp);
            eManager->setComponentData<TypeComponent>(id, type);
            eManager->setComponentData<CollisionComponent>(id, colComp);
            eManager->setComponentData<RenderComponent>(id, textureComp);
            eManager->setComponentData<GUIComponent>(id, guiComp);
            //LevelUpContainer
            std::string text = getNextUpgradeText(playerComp, options[i]);
            lvlUpContainers.push_back(std::make_unique<LevelUpContainer>(
                eManager,
                options[i],
                text,
                pos,
                renderer
            ));
        }
    }
}

void LevelUpScreen::destroy(EntityManager* eManager) {
    // std::cout << "Destroying LevelUpScreen" << std::endl;
    eManager->destroyEntityLater(id1);
    eManager->destroyEntityLater(id2);
    eManager->destroyEntityLater(id3);
    for (int i = 0; i < lvlUpContainers.size(); i++) {
        eManager->destroyEntityLater(lvlUpContainers.at(i)->containerId);
        eManager->destroyEntityLater(lvlUpContainers.at(i)->textId);
        eManager->destroyEntityLater(lvlUpContainers.at(i)->iconId);
    }
}

void LevelUpScreen::handleMouseHover(std::shared_ptr<MouseMotionMessage> msg) {
    handleHover(id1, msg->mousePos, nullptr);
    if (options.size() > 1) handleHover(id2, msg->mousePos, nullptr);
    if (options.size() > 2) handleHover(id3, msg->mousePos, nullptr);
}

void LevelUpScreen::handleMouseClick(std::shared_ptr<ClickMessage> msg) {
    handleClick(id1, msg->mousePos, [this]() {
        onContainerClick(0);
    });
    if (options.size() > 1) handleClick(id2, msg->mousePos, [this]() {
        onContainerClick(1);
    });
    if (options.size() > 2) handleClick(id3, msg->mousePos, [this]() {
        onContainerClick(2);
    });
}

void LevelUpScreen::update(EntityManager* eManager, SDL_Renderer* renderer) {
    
}

void LevelUpScreen::drawConatiner(SDL_Renderer* renderer, SDL_Texture* texture) {
    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer, Colors::White.r, Colors::White.g, Colors::White.b, Colors::White.a);
    SDL_FRect rect = {0, 0, GUI::CONTAINER_WIDTH + 4, GUI::CONTAINER_HEIGHT + 4};
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderTarget(renderer, NULL);
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
}

void LevelUpScreen::onContainerClick(int i) {
    MessageManager::getInstance().sendMessage(std::make_shared<LevelUpMessage>(options[i]));
    GameStateManager::getInstance().setState(GameState::Playing);
}
