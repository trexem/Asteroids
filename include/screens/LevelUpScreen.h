#pragma once

#include <vector>
#include <memory>

#include "Screen.h"
#include "LevelUpContainer.h"
#include "LevelUpMessage.h"
#include "MessageManager.h"
#include "screens/Components/Button.h"

class KeyboardMessage;

class LevelUpScreen : public Screen {
    std::vector<std::shared_ptr<Button>> containers;
    std::shared_ptr<Button> levelUpButton;
    std::vector<std::unique_ptr<LevelUpContainer>> lvlUpContainers;
    std::vector<AbilityChoice> options;
    int selected {-1}, newSelected{0};

    void handleMouseHover(std::shared_ptr<MouseMotionMessage> msg);
    void handleMouseClick(std::shared_ptr<ClickMessage> msg);
    void handleKeyboardMessage(std::shared_ptr<KeyboardMessage> msg);

    void drawConatiner(SDL_Renderer* renderer, SDL_Texture* texture);
    void onContainerClick(int i);

public:
    LevelUpScreen(EntityManager& eManager) : Screen(eManager) {}
    void create(EntityManager& eManager, SDL_Renderer* renderer);
    void destroy(EntityManager& eManager);
    void update(EntityManager& eManager, SDL_Renderer* renderer);
};
