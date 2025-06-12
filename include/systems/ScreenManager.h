#pragma once

#include <iostream>
#include <chrono>

#include "GameStateManager.h"
#include "KeyboardMessage.h"
#include "GameOverScreen.h"
#include "MainMenuScreen.h"
#include "LevelUpScreen.h"
#include "PauseScreen.h"
#include "PlayingScreen.h"
#include "SettingsScreen.h"
#include "System.h"

class ScreenManager : public System {
public:
    ScreenManager(EntityManager& eM, SDL_Renderer* renderer);
    ~ScreenManager();

    void update(EntityManager& eMgr, const double& dT) override;

private:
    void changeScreen(GameState newState);
    void updateState();
    bool isOverlayState(GameState state);
    EntityManager& eMgr;
    SDL_Renderer* renderer;
    GameState currentGameState{GameState::Quit};
    GameState currentMainState{GameState::Quit};
    GameState currentOverlayState{GameState::Quit};
    std::shared_ptr<Screen> screen = nullptr;
    std::shared_ptr<Screen> overlayScreen = nullptr;
};
