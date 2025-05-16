#include "ClickMessage.h"
#include "FPair.h"
#include "GameStateManager.h"
#include "InputSystem.h"
#include "KeyboardMessage.h"
#include "MouseMotionMessage.h"
#include "MessageManager.h"
#include "SettingsManager.h"

void InputSystem::update(EntityManager& eMgr, const double& dT) {
    (void) eMgr; // unused
    (void) dT; // unused
    while (SDL_PollEvent(&e)) {
		//Quit if the X button is pressed
        switch (e.type) {
        case SDL_EVENT_QUIT: {
            GameStateManager::instance().setState(GameState::Quit);
            break;
        }
        case SDL_EVENT_KEY_DOWN:
        case SDL_EVENT_KEY_UP: {
            bool pressed = (e.type == SDL_EVENT_KEY_DOWN);
            auto msg = std::make_shared<KeyboardMessage>(e.key.key, pressed);
            MessageManager::instance().sendMessage(msg);
            break;
        }
        case SDL_EVENT_MOUSE_MOTION: {
            FPair rawMousePos(e.motion.x, e.motion.y);
            FPair mousePos = adjustMousePos(rawMousePos);
            auto msg = std::make_shared<MouseMotionMessage>(mousePos);
            MessageManager::instance().sendMessage(msg);
            break;
        }
        case SDL_EVENT_MOUSE_BUTTON_UP:
        case SDL_EVENT_MOUSE_BUTTON_DOWN: {
            bool isDown = e.type == SDL_EVENT_MOUSE_BUTTON_DOWN;
            FPair rawMousePos(e.button.x, e.button.y);
            FPair mousePos = adjustMousePos(rawMousePos);
            clickHandler.handleClick(mousePos, isDown, e.button.button);
            break;
        }
        default:
            break;
        } 
	}
}

FPair InputSystem::adjustMousePos(FPair mousePos) {
    return mousePos / SettingsManager::instance().scale;
}
