#pragma once
#ifndef __PLAYER_SYSTEM_H_
#define __PLAYER_SYSTEM_H_

#include <iostream>

#include "System.h"
#include "MessageManager.h"
#include "KeyboardMessage.h"

class PlayerSystem : public System {
public:
    PlayerSystem() {
        std::cout << " PlayerSystem Subscribing to: ";
        // Subscribe to KeyboardMessages
        MessageManager::getInstance().subscribe<KeyboardMessage>(
            [this](std::shared_ptr<KeyboardMessage> msg) { handleKeyboardInput(msg); }
        );
    }

private:
    void handleKeyboardInput(std::shared_ptr<KeyboardMessage> msg) {
        if (msg->pressed) {
            std::cout << "Player system Key pressed: " << SDL_GetKeyName(msg->key) << std::endl;
        }
    }
};

#endif // !__PLAYER_SYSTEM_H_