#include "ClickMessage.h"
#include "GameStateManager.h"
#include "LevelUpScreen.h"
#include "MouseMotionMessage.h"
#include "TextureManager.h"
#include "TextManager.h"
#include "KeyboardMessage.h"

void LevelUpScreen::create(EntityManager& eManager, SDL_Renderer* renderer) {
    auto weak_this = std::weak_ptr<Screen>(shared_from_this());
    this->unsubscribers.emplace_back(
        MessageManager::instance().subscribe<KeyboardMessage>(
            [weak_this](auto msg) {
                if (auto shared_this = weak_this.lock()) {
                    auto* ptr = dynamic_cast<LevelUpScreen*>(shared_this.get());
                    if (ptr) {
                        ptr->handleKeyboardMessage(msg);
                    }
                }
            }
        )
    );

    TypeComponent type = EntityType::GUI;
    TransformComponent trComp;
    CollisionComponent colComp;
    GUIComponent guiComp;
    FPair pos, size;
    size = {GUI::containerWidth, GUI::containerHeight};
    Texture* texture = TextureManager::instance().get("gui/tooltip");
    NineGridComponent grid {16.0f, 16.0f, 16.0f, 16.0f};
    ClickCallbackComponent callback;
    pos.y = GUI::screenHeight * 0.1851f;
    auto players = eManager.getEntitiesWithComponent(ComponentType::Player);
    for (uint32_t player : players) {
        PlayerComponent playerComp = eManager.getComponentData<PlayerComponent>(player);
        options = getRandomAbilityChoices(playerComp);
        if (options.size() == 0) {
            options.push_back({AbilityType::Money, static_cast<size_t>(PassiveAbilities::PassiveAbilitiesCount)});
            options.push_back({AbilityType::Health, static_cast<size_t>(PassiveAbilities::PassiveAbilitiesCount) + 1});
        }
        size_t count = options.size();
        for (int i = 0; i < count; i++) {
            if (count == 3) {
                pos.x = GUI::screenWidth * .15625f + i * (GUI::containerWidth + GUI::containerGap);
            } else {
                pos.x = i == 0 ?
                GUI::screenCenter.x - GUI::containerWidth - GUI::containerGap :
                GUI::screenCenter.x + GUI::containerGap;
            }
            //Container
            containers.push_back(std::make_shared<Button>(
                eManager, "", pos - hoveredOffset - 5, size + 4 * hoveredOffset, texture, grid, renderer
            ));
            callback.onClick = [&, i] (uint32_t entity) {
                newSelected = i;
            };
            eManager.addComponent(containers[i]->id, ComponentType::ClickCallback);
            eManager.setComponentData(containers[i]->id, callback);

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
    // LevelUp Button
    pos = {GUI::screenCenter.x - 200, pos.y + GUI::containerHeight + 50.0f};
    size = {400, 100};
    std::string text = TextManager::instance().get("action.levelUp");
    levelUpButton = std::make_shared<Button>(eManager, text, pos, size, texture, grid, renderer);
    callback.onClick = [&] (uint32_t entity) {
        MessageManager::instance().sendMessage(std::make_shared<LevelUpMessage>(options[selected]));
        GameStateManager::instance().setState(GameState::Playing);
    };
    eManager.addComponent(levelUpButton->id, ComponentType::ClickCallback);
    eManager.setComponentData(levelUpButton->id, callback);
}

void LevelUpScreen::destroy(EntityManager& eManager) {
    // std::cout << "Destroying LevelUpScreen" << std::endl;
    for (auto container : containers) {
        container->destroy(eManager);
    }
    for (int i = 0; i < lvlUpContainers.size(); i++) {
        eManager.destroyEntityLater(lvlUpContainers.at(i)->containerId);
        eManager.destroyEntityLater(lvlUpContainers.at(i)->textId);
        eManager.destroyEntityLater(lvlUpContainers.at(i)->iconId);
        lvlUpContainers[i]->destroy(eManager);
    }
    levelUpButton->destroy(eManager);
}

void LevelUpScreen::handleMouseHover(std::shared_ptr<MouseMotionMessage> msg) {
}

void LevelUpScreen::handleMouseClick(std::shared_ptr<ClickMessage> msg) {
}

void LevelUpScreen::update(EntityManager& eManager, SDL_Renderer* renderer) {
    if (newSelected != selected) {
        selected = newSelected;
        for (int i = 0; i < containers.size(); i++) {
            auto* state = eManager.getComponentDataPtr<GUIStateComponent>(containers[i]->id);
            state->state = i == selected ? GUIState::Selected : GUIState::Idle;
        }
    }
    for (auto container : containers) {
        container->updateState(eManager);
    }
    levelUpButton->updateState(eManager);
}

void LevelUpScreen::drawConatiner(SDL_Renderer* renderer, SDL_Texture* texture) {
    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer, Colors::White.r, Colors::White.g, Colors::White.b, Colors::White.a);
    SDL_FRect rect = {0, 0, GUI::containerWidth + 4, GUI::containerHeight + 4};
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderTarget(renderer, NULL);
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
}

void LevelUpScreen::onContainerClick(int i) {
    newSelected = i;
    MessageManager::instance().sendMessage(std::make_shared<LevelUpMessage>(options[i]));
    GameStateManager::instance().setState(GameState::Playing);
}

void LevelUpScreen::handleKeyboardMessage(std::shared_ptr<KeyboardMessage> msg) {
    if (!msg->pressed) {
        switch (msg->key) {
        case SDLK_A:
        case SDLK_LEFT:
            if (newSelected > 0) {
                newSelected--;
            }
            break;
        case SDLK_D:
        case SDLK_RIGHT:
            if (newSelected < options.size() - 1) {
                newSelected++;
            }
            break;
        case SDLK_RETURN:
        case SDLK_SPACE:
            MessageManager::instance().sendMessage(std::make_shared<LevelUpMessage>(options[selected]));
            GameStateManager::instance().setState(GameState::Playing);
            break;
        default:
            break;
        }
    }
}
