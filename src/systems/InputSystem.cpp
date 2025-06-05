#include "ClickMessage.h"
#include "FPair.h"
#include "GameStateManager.h"
#include "InputSystem.h"
#include "KeyboardMessage.h"
#include "MouseMotionMessage.h"
#include "MessageManager.h"
#include "SettingsManager.h"
#include "TouchMessage.h"

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
#if not defined(__ANDROID__)
        case SDL_EVENT_MOUSE_BUTTON_UP:
        case SDL_EVENT_MOUSE_BUTTON_DOWN: {
            bool isDown = e.type == SDL_EVENT_MOUSE_BUTTON_DOWN;
            FPair rawMousePos(e.button.x, e.button.y);
            DEBUG_LOG("Raw mouse pos (%.2f, %.2f)", rawMousePos.x, rawMousePos.y);
            FPair mousePos = adjustMousePos(rawMousePos);
            DEBUG_LOG("Adjusted mouse pos (%.2f, %.2f)", mousePos.x, mousePos.y);
            clickHandler.handleClick(mousePos, isDown, e.button.button);
            break;
        }
#endif // !__ANROID__
        case SDL_EVENT_FINGER_DOWN:
        case SDL_EVENT_FINGER_MOTION:
        case SDL_EVENT_FINGER_UP: {
            FPair touchPos(e.tfinger.x, e.tfinger.y);
            FPair pos = adjustTouchPos(touchPos);
            bool isDown = (e.type == SDL_EVENT_FINGER_DOWN || e.type == SDL_EVENT_FINGER_MOTION);
            auto msg = std::make_shared<TouchMessage>(pos, isDown, e.tfinger.fingerID);
            MessageManager::instance().sendMessage(msg);
            clickHandler.handleClick(pos, isDown, 1); // simulate left mouse button
            break;
        }
        default:
            break;
        } 
	}
}

FPair InputSystem::adjustMousePos(FPair mousePos) {
    return (mousePos - SettingsManager::instance().screenPos) / SettingsManager::instance().scale;
}

FPair InputSystem::adjustTouchPos(FPair pos) {
    const FPair& size = SettingsManager::instance().currentScreenSize;
    const FPair& offset = SettingsManager::instance().screenPos;
    const FPair& scale = SettingsManager::instance().scale;
    DEBUG_LOG("ScreenSize (%.2f, %.2f)", size.x, size.y);
    DEBUG_LOG("Screen pos (%.2f, %.2f)", offset.x, offset.y);
    DEBUG_LOG("Scale (%.2f, %.2f)", scale.x, scale.y);
    return (pos * size - offset) / scale;
}
