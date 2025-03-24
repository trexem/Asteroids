#pragma once
#ifndef __UI_SYSTEM_H_
#define __UI_SYSTEM_H_

#include <iostream>

#include "System.h"
#include "MessageManager.h"
#include "KeyboardMessage.h"

class UISystem : public System {
public:
    UISystem() {
        std::cout << "UISystem subscribing to: " << typeid(KeyboardMessage).name() 
          << " (" << typeid(KeyboardMessage).hash_code() << ")" << std::endl;
        // Subscribe to KeyboardMessages
        MessageManager::getInstance().subscribe<KeyboardMessage>(
            [this](std::shared_ptr<KeyboardMessage> msg) { handleKeyboardInput(msg); }
        );
    }

private:
    void handleKeyboardInput(std::shared_ptr<KeyboardMessage> msg) {
        if (msg->pressed) {
            // std::cout << "UI system Key pressed: " << SDL_GetKeyName(msg->key) << std::endl;
        }
    }
};

#endif // !__UI_SYSTEM_H_