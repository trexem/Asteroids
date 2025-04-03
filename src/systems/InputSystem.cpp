#include "InputSystem.h"

void InputSystem::update() {
    while (SDL_PollEvent(&e)) {
		//Quit if the X button is pressed
        switch (e.type) {
        case SDL_EVENT_QUIT: {
            GameStateManager::getInstance().setState(GameState::Quit);
            break;
        }
        case SDL_EVENT_KEY_DOWN:
        case SDL_EVENT_KEY_UP: {
            bool pressed = (e.type == SDL_EVENT_KEY_DOWN);
            auto msg = std::make_shared<KeyboardMessage>(e.key.key, pressed);
            MessageManager::getInstance().sendMessage(msg);
            break;
        }
        case SDL_EVENT_MOUSE_MOTION: {
            FPair mousePos(e.motion.x, e.motion.y);
            auto msg = std::make_shared<MouseMotionMessage>(mousePos);
            MessageManager::getInstance().sendMessage(msg);
            break;
        }
        case SDL_EVENT_MOUSE_BUTTON_DOWN: {
            FPair mousePos(e.button.x, e.button.y);
            clickHandler.handleClick(mousePos, e.button.button, e.button.clicks);
            break;
        }
        default:
            break;
        } 
	}
}
