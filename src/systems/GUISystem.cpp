#include "GUISystem.h"

GUISystem::GUISystem(EntityManager* eM, SDL_Renderer* renderer) 
    : eManager(eM), renderer(renderer) {
    // std::cout << "GUISystem subscribing to: " << typeid(KeyboardMessage).name() 
    // << " (" << typeid(KeyboardMessage).hash_code() << ")" << std::endl;
    // Subscribe to GameStateMessage
    // MessageManager::getInstance().subscribe<GameStateMessage>(
    //     [this](std::shared_ptr<GameStateMessage> msg) { updateState(); }
    // );
}

GUISystem::~GUISystem() {
    if (overlayScreen) {
        overlayScreen->clearSubscriptions();
        overlayScreen->destroy(eManager);
    }
    screen->clearSubscriptions();
    screen->destroy(eManager);
    overlayScreen = nullptr;
    screen = nullptr;
}

void GUISystem::update() {
    auto start = std::chrono::high_resolution_clock::now();
    updateState();
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "GUISystem updateState time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " us\n";
    if (overlayScreen) {
        overlayScreen->update(eManager, renderer);
    }
    start = std::chrono::high_resolution_clock::now();
    screen->update(eManager, renderer);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "GUISystem screenUpdate time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " us\n";
}

void GUISystem::updateState() {
    GameState state = GameStateManager::getInstance().getState();
    // std::cout << "received state: " << int(state) << std::endl;
    // std::cout << "Current state: " << int(currentGameState) << std::endl;
    // If state has changed, update UI
    if (state != currentGameState) {    
        changeScreen(state);
    }
}

void GUISystem::changeScreen(GameState newState) {
    if ((newState == GameState::LevelUp || newState == GameState::Paused)) {
        // Only add overlay
        if (!overlayScreen || newState != currentOverlayState) {
                if (overlayScreen) {
                overlayScreen->clearSubscriptions();
                overlayScreen->destroy(eManager);
            }
            switch (newState) {
            case GameState::LevelUp:
                overlayScreen = std::make_shared<LevelUpScreen>(eManager);
                break;
            case GameState::Paused:
                overlayScreen = std::make_shared<PauseScreen>(eManager);
            default:
                break;
            }
            overlayScreen->initSubscriptions();
            overlayScreen->create(eManager, renderer);
            currentOverlayState = newState;
        }
    } else {
        // Destroy overlay if it exists
        if (overlayScreen) {
            overlayScreen->clearSubscriptions();
            overlayScreen->destroy(eManager);
            overlayScreen = nullptr;
        }
        // Only recreate base screen if different
        if (!screen || newState != currentMainState) {
            if (screen) {
                screen->clearSubscriptions();
                screen->destroy(eManager);
            }
            switch (newState) {
                case GameState::MainMenu:
                    screen = std::make_shared<MainMenuScreen>(eManager);
                    break;
                case GameState::Settings:
                    screen = std::make_shared<SettingsScreen>(eManager);
                    break;
                case GameState::Playing:
                    screen = std::make_shared<PlayingScreen>(eManager);
                    break;
                case GameState::GameOver:
                    screen = std::make_shared<GameOverScreen>(eManager);
                    break;
                default:
                    break;
            }

            // Initialize all screens
            if (screen) {
                screen->initSubscriptions();
                screen->create(eManager, renderer);
            }
            currentMainState = newState;
        }
    }
    currentGameState = newState;
}
