#pragma once

#include "Colors.h"
#include "Fonts.h"
#include "Screen.h"
#include "texture.hpp"
#include "screens/Components/Button.h"

class GameOverScreen : public Screen {
    std::unique_ptr<Button> restartButton;
    std::unique_ptr<Button> mainMenuButton;
    std::unique_ptr<Button> exitButton;
    uint32_t restartID, mmID, quitID;

    void handleMouseHover(std::shared_ptr<MouseMotionMessage> msg);
    void handleMouseClick(std::shared_ptr<ClickMessage> msg);

public:
    GameOverScreen(EntityManager& eManager) : Screen(eManager) {}
    void create(EntityManager& eManager, SDL_Renderer* renderer);
    void destroy(EntityManager& eManager);
    void update(EntityManager& eManager, SDL_Renderer* renderer);
};
