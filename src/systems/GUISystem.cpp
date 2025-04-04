#include "GUISystem.h"

GUISystem::GUISystem(EntityManager* em, SDL_Renderer* renderer) 
    : eManager(em), renderer(renderer) {
    // std::cout << "GUISystem subscribing to: " << typeid(KeyboardMessage).name() 
    // << " (" << typeid(KeyboardMessage).hash_code() << ")" << std::endl;
    // Subscribe to MouseMotionMessages
    MessageManager::getInstance().subscribe<MouseMotionMessage>(
        [this](std::shared_ptr<MouseMotionMessage> msg) { handleMouseHover(msg); }
    );
    // Subscribe to ClickMessages
    MessageManager::getInstance().subscribe<ClickMessage>(
        [this](std::shared_ptr<ClickMessage> msg) { handleMouseClick(msg); }
    );
    currentGameState = GameStateManager::getInstance().getState();
}

void GUISystem::handleMouseHover(std::shared_ptr<MouseMotionMessage> msg) {
    std::cout << "Mouse position: " << msg->mousePos.x << ", " 
        << msg->mousePos.x << std::endl;
}

void GUISystem::handleMouseClick(std::shared_ptr<ClickMessage> msg) {
    std::cout << "Mouse click: " << msg->mousePos.x << ", " 
        << msg->mousePos.x << " with button: " <<  int(msg->button)
        << " and clicks: " << int(msg->clicks) << std::endl;
}

void GUISystem::update() {
    GameState state = GameStateManager::getInstance().getState();

    // If state has changed, update UI
    if (state != currentGameState) {
        changeScreen(state);
        currentGameState = state;
    }
}

void GUISystem::changeScreen(GameState newState) {
    if (currentScreen) {
        currentScreen->destroy(eManager);
        delete currentScreen;
    }

    switch (newState) {
    case GameState::MainMenu:
        currentScreen = new MainMenuScreen();
        break;
    case GameState::Settings:
        currentScreen = new SettingsScreen();
        break;
    case GameState::Playing:
        currentScreen = new PlayingScreen();
        break;
    case GameState::GameOver:
        currentScreen = new GameOverScreen();
        break;
    default:
        currentScreen = nullptr;
        break;
    }

    if (currentScreen) {
        currentScreen->create(eManager, renderer);
    }
}
