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
        case SDL_EVENT_FINGER_DOWN:
        case SDL_EVENT_FINGER_MOTION:
        case SDL_EVENT_FINGER_UP: {
            float normX = e.tfinger.x;
            float normY = e.tfinger.y;
            float screenX = normX * SettingsManager::instance().currentScreenSize.x;
            float screenY = normY * SettingsManager::instance().currentScreenSize.y;
            FPair pos(screenX, screenY);

            bool isDown = (e.type == SDL_EVENT_FINGER_DOWN || e.type == SDL_EVENT_FINGER_MOTION);

            // Example: left side of screen = joystick
            if (screenX < SettingsManager::instance().currentScreenSize.x * 0.4f) {
                // send to JoystickSystem (to be created)
                // maybe store fingerId and position for dragging logic
            } else {
                // treat it as a click
                clickHandler.handleClick(pos, isDown, 1); // simulate left mouse button
            }
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
