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
    std::mutex clickMutex;
    bool waitingForDoubleClick = false;
    std::thread clickThread;

public:
    void handleClick(FPair pos, int button, int clickCount) {
        std::lock_guard<std::mutex> lock(clickMutex);

        if (clickCount == 2) {
            // Cancel single-click action and process double-click immediately
            waitingForDoubleClick = false;
            auto msg = std::make_shared<ClickMessage>(pos, button, 2);
            MessageManager::instance().sendMessage(msg);
        } else {
            // Wait before processing to see if a second click happens
            waitingForDoubleClick = true;
            if (clickThread.joinable()) clickThread.join(); // Ensure no previous thread is running

            clickThread = std::thread([this, pos, button]() {
                std::this_thread::sleep_for(std::chrono::milliseconds(200)); // Wait for possible second click

                std::lock_guard<std::mutex> lock(clickMutex);
                if (waitingForDoubleClick) {
                    // If still waiting, process as a single click
                    auto msg = std::make_shared<ClickMessage>(pos, button, 1);
                    MessageManager::instance().sendMessage(msg);
                    waitingForDoubleClick = false;
                }
            });
        }
    }

    ~ClickHandler() {
        if (clickThread.joinable()) clickThread.join();
    }
};
