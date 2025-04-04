#pragma once

#include <iostream>

#include "GameStateManager.h"
#include "KeyboardMessage.h"
#include "GameOverScreen.h"
#include "MainMenuScreen.h"
#include "LevelUpScreen.h"
#include "PlayingScreen.h"
#include "SettingsScreen.h"
#include "System.h"

class GUISystem : public System {
public:
    GUISystem(EntityManager* eM, SDL_Renderer* renderer);
    ~GUISystem();

    void update();

private:
    void changeScreen(GameState newState);
    EntityManager* eManager;
    SDL_Renderer* renderer;
    GameState currentGameState{GameState::Quit};
    std::vector<std::shared_ptr<Screen>> screens;
};
