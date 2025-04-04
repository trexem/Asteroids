#pragma once

#include <iostream>

#include "ClickMessage.h"
#include "GameStateManager.h"
#include "KeyboardMessage.h"
#include "MessageManager.h"
#include "MouseMotionMessage.h"
#include "GameOverScreen.h"
#include "MainMenuScreen.h"
#include "PlayingScreen.h"
#include "SettingsScreen.h"
#include "System.h"

class GUISystem : public System {
public:
    GUISystem(EntityManager* em, SDL_Renderer* renderer);

    void update();

private:
    void handleMouseHover(std::shared_ptr<MouseMotionMessage> msg);
    void handleMouseClick(std::shared_ptr<ClickMessage> msg);
    void changeScreen(GameState newState);
    EntityManager* eManager;
    SDL_Renderer* renderer;
    GameState currentGameState;
    Screen* currentScreen {nullptr};
};
