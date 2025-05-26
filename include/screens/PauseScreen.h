#pragma once

#include "screens/Components/Button.h"
#include "Screen.h"
#include "texture.hpp"

class PauseScreen : public Screen {
    std::unique_ptr<Button> continueButton;
    std::unique_ptr<Button> settingsButton;
    std::unique_ptr<Button> mainMenuButton;
    std::unique_ptr<Button> restartButton;
    std::unique_ptr<Button> exitButton;
    

    void handleMouseHover(std::shared_ptr<MouseMotionMessage> msg);
    void handleMouseClick(std::shared_ptr<ClickMessage> msg);

public:
    PauseScreen(EntityManager& eManager) : Screen(eManager) {}
    void create(EntityManager& eManager, SDL_Renderer* renderer);
    void destroy(EntityManager& eManager);
    void update(EntityManager& eManager, SDL_Renderer* renderer);
};
