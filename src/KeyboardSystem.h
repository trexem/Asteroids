#pragma once
#ifndef __KEYBOARD_SYSTEM_H_
#define __KEYBOARD_SYSTEM_H_

#include "System.h"
#include "MessageManager.h"
#include "KeyboardMessage.h"
#include <SDL3/SDL.h>

class KeyboardSystem : public System {
public:
    void handleEvent(const SDL_Event& event) {
        if (event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_KEY_UP) {
            bool pressed = (event.type == SDL_EVENT_KEY_DOWN);
            auto msg = std::make_shared<KeyboardMessage>(event.key.key, pressed);
            MessageManager::getInstance().sendMessage(msg);
        }
    }
};

#endif // !__KEYBOARD_SYSTEM_H_