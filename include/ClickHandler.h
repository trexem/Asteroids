#pragma once

#include "FPair.h"
#include "ClickMessage.h"
#include "MessageManager.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <functional>
#include <mutex>

class ClickHandler {
private:
    std::chrono::time_point<std::chrono::steady_clock> lastClickTime;
    FPair lastClickPos;
    int lastClickButton = -1;
    bool wasDown = false;
    const int doubleClickDelayMs = 300; // Adjust to match system settings

public:
    void handleClick(FPair pos, bool isDown, int button) {
        const auto now = std::chrono::steady_clock::now();
        const auto timeSinceLastClick = 
            std::chrono::duration_cast<std::chrono::milliseconds>(
                now - lastClickTime
            ).count();

        // Handle button down/up transitions
        if (isDown && !wasDown) {
            // Check for double click
            if (button == lastClickButton && 
                pos == lastClickPos && 
                timeSinceLastClick < doubleClickDelayMs) {
                
                // Double click detected
                auto msg = std::make_shared<ClickMessage>(pos, true, button, 2);
                MessageManager::instance().sendMessage(msg);
                lastClickButton = -1; // Reset to prevent triple-click as another double
            } else {
                // Single click down (wait for up to confirm)
                lastClickTime = now;
                lastClickPos = pos;
                lastClickButton = button;
            }
        } else if (!isDown && wasDown && button == lastClickButton) {
            // Single click completed
            auto msg = std::make_shared<ClickMessage>(pos, false, button, 1);
            MessageManager::instance().sendMessage(msg);
        }

        wasDown = isDown;
    }
};
