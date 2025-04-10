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
    for (auto& screen : screens) {
        screen->clearSubscriptions();
        screen->destroy(eManager);
    }
    screens.clear();
}

void GUISystem::update() {
    updateState();
    for (auto& screen : screens) {
        screen->update(eManager, renderer);
    }
}

void GUISystem::updateState() {
    GameState state = GameStateManager::getInstance().getState();
    std::cout << "received state: " << int(state) << std::endl;
    std::cout << "Current state: " << int(currentGameState) << std::endl;
    // If state has changed, update UI
    if (state != currentGameState) {
        currentGameState = state;
        changeScreen(state);
    }
}

void GUISystem::changeScreen(GameState newState) {
    // Clear all screens before adding the new one (unless it's an overlay)
    if (newState != GameState::LevelUp) { // LevelUp is an overlay
        for (auto& screen : screens) {
            screen->clearSubscriptions();
            std::cout << "Type before destroy: " << typeid(*screen).name() << "\n";
            screen->destroy(eManager);
            std::cout << "Reference count after clear: " << screen.use_count() << "\n";
        }
        screens.clear();
    }

    switch (newState) {
        case GameState::MainMenu:
            screens.push_back(std::make_shared<MainMenuScreen>(eManager));
            break;
        case GameState::Settings:
            screens.push_back(std::make_shared<SettingsScreen>(eManager));
            break;
        case GameState::Playing:
            screens.push_back(std::make_shared<PlayingScreen>(eManager));
            break;
        case GameState::GameOver:
            screens.push_back(std::make_shared<GameOverScreen>(eManager));
            break;
        case GameState::LevelUp: // Overlay case
            screens.push_back(std::make_shared<LevelUpScreen>(eManager));
            break;
        default:
            break;
    }

    // Initialize all screens
    for (auto& screen : screens) {
        screen->initSubscriptions();
        screen->create(eManager, renderer);
    }
}
