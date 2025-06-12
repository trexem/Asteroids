#include "ScreenManager.h"
#include "UpgradeStoreScreen.h"

ScreenManager::ScreenManager(EntityManager& eM, SDL_Renderer* renderer) 
    : eMgr(eM), renderer(renderer) {
}

ScreenManager::~ScreenManager() {
    if (overlayScreen) {
        overlayScreen->clearSubscriptions();
        overlayScreen->destroy(eMgr);
    }
    if (screen) {
        screen->clearSubscriptions();
        screen->destroy(eMgr);
    }
    overlayScreen = nullptr;
    screen = nullptr;
}

void ScreenManager::update(EntityManager& eMgr, const double& dT) {
    (void)dT; // unused
    updateState();
    if (overlayScreen) {
        overlayScreen->update(eMgr, renderer);
    }
    if (screen) {
        screen->update(eMgr, renderer);
    }
}

void ScreenManager::updateState() {
    GameState state = GameStateManager::instance().getState();
    if (state != currentGameState) {    
        changeScreen(state);
    }
}

void ScreenManager::changeScreen(GameState newState) {
    if (isOverlayState(newState)) {
        // Only add overlay
        if (!overlayScreen || newState != currentOverlayState) {
            if (overlayScreen) {
                overlayScreen->clearSubscriptions();
                overlayScreen->destroy(eMgr);
            }
            switch (newState) {
            case GameState::MainMenu:
                if (screen) {
                    screen->clearSubscriptions();
                    screen->destroy(eMgr);
                    screen.reset();
                }
                overlayScreen = std::make_shared<MainMenuScreen>(eMgr);
                break;
            case GameState::Settings:
                overlayScreen = std::make_shared<SettingsScreen>(eMgr);
                break;
            case GameState::UpgradeStore:
                overlayScreen = std::make_shared<UpgradeStoreScreen>(eMgr, renderer);
                break;
            case GameState::LevelUp:
                overlayScreen = std::make_shared<LevelUpScreen>(eMgr);
                break;
            case GameState::Paused:
                overlayScreen = std::make_shared<PauseScreen>(eMgr);
                break;
            case GameState::GameOver:
                if (screen) {
                    screen->clearSubscriptions();
                    screen->destroy(eMgr);
                    screen.reset();
                }
                overlayScreen = std::make_shared<GameOverScreen>(eMgr);
                break;
            default:
                break;
            }
            overlayScreen->initSubscriptions();
            overlayScreen->create(eMgr, renderer);
            currentOverlayState = newState;
        }
    } else {
        // Destroy overlay if it exists
        if (overlayScreen) {
            overlayScreen->clearSubscriptions();
            overlayScreen->destroy(eMgr);
            overlayScreen = nullptr;
        }
        // Only recreate base screen if different
        if (!screen || newState != currentMainState) {
            if (screen) {
                screen->clearSubscriptions();
                screen->destroy(eMgr);
            }
            switch (newState) {
                case GameState::Playing:
                    screen = std::make_shared<PlayingScreen>(eMgr);
                    break;
                default:
                    break;
            }

            // Initialize all screens
            if (screen) {
                screen->initSubscriptions();
                screen->create(eMgr, renderer);
            }
            currentMainState = newState;
        }
    }
    currentGameState = newState;
}

bool ScreenManager::isOverlayState(GameState state) {
    switch (state) {
    case GameState::LevelUp:
    case GameState::Paused:
    case GameState::MainMenu:
    case GameState::Settings:
    case GameState::UpgradeStore:
    case GameState::GameOver:
        return true;
    default:
        return false;
    }
}
