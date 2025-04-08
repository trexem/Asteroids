#pragma once

#include <queue>
#include <memory>
#include <functional>
#include <unordered_map>
#include <iostream>
#include <vector>

#include "message.h"

class MessageManager {
public:
    static MessageManager& getInstance();

    template<typename T>
    std::function<void()> subscribe(std::function<void(std::shared_ptr<T>)> callback) {
        size_t type = typeid(T).hash_code();
        auto& subscribers = listeners[type];
        
        // Add the callback
        subscribers.push_back([callback](std::shared_ptr<Message> msg) {
            callback(std::static_pointer_cast<T>(msg));
        });
        
        // Return an unsubscriber
        return [this, type, &subscribers, callback]() {
            subscribers.erase(
                std::remove_if(
                    subscribers.begin(),
                    subscribers.end(),
                    [&callback](const auto& stored) {
                        return stored.target_type() == callback.target_type();
                    }
                ),
                subscribers.end()
            );
        };
    }

    template<typename T>
    void sendMessage(std::shared_ptr<T> message) {
        size_t type = typeid(T).hash_code(); // Use the exact message type!
        /*std::cout << "Sending Message of type: " << typeid(T).name()
                << " (" << type << ")" << std::endl;*/

        if (listeners.find(type) != listeners.end()) {
            //std::cout << "Found listener for type " << type << std::endl;
            for (auto& callback : listeners[type]) {
                callback(message);
            }
        } else {
            //std::cout << "No listener found for type " << type << std::endl;
        }
    }

    void cleanup() {
        std::cout << "Cleaning up MessageManager\n";
        listeners.clear();
    }
private:
    MessageManager() = default;
    ~MessageManager() = default;

    MessageManager(const MessageManager&) = delete;
    MessageManager& operator=(const MessageManager&) = delete;

    std::unordered_map<size_t, std::vector<std::function<void(std::shared_ptr<Message>)>>> listeners;
};
