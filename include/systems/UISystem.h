#pragma once
#ifndef __UI_SYSTEM_H_
#define __UI_SYSTEM_H_

#include <iostream>

#include "System.h"
#include "MessageManager.h"
#include "KeyboardMessage.h"
#include "MouseMotionMessage.h"
#include "ClickMessage.h"

class UISystem : public System {
public:
    UISystem() {
        std::cout << "UISystem subscribing to: " << typeid(KeyboardMessage).name() 
          << " (" << typeid(KeyboardMessage).hash_code() << ")" << std::endl;
        // Subscribe to MouseMotionMessages
        MessageManager::getInstance().subscribe<MouseMotionMessage>(
            [this](std::shared_ptr<MouseMotionMessage> msg) { handleMouseHover(msg); }
        );
        // Subscribe to ClickMessages
        MessageManager::getInstance().subscribe<ClickMessage>(
            [this](std::shared_ptr<ClickMessage> msg) { handleMouseClick(msg); }
        );
    }

private:
    void handleMouseHover(std::shared_ptr<MouseMotionMessage> msg) {
        std::cout << "Mouse position: " << msg->mousePos.x << ", " 
            << msg->mousePos.x << std::endl;
    }

    void handleMouseClick(std::shared_ptr<ClickMessage> msg) {
        std::cout << "Mouse click: " << msg->mousePos.x << ", " 
            << msg->mousePos.x << " with button: " <<  int(msg->button)
            << " and clicks: " << int(msg->clicks) << std::endl;
    }
};

#endif // !__UI_SYSTEM_H_