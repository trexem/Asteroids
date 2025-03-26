#pragma once
#ifndef __MESSAGE_MANAGER_H_
#define __MESSAGE_MANAGER_H_

#include <queue>
#include <memory>
#include <functional>
#include <unordered_map>

#include "Message.h"

class MessageManager {
public:
    static MessageManager& getInstance();

    void sendMessage(std::shared_ptr<Message> message);
    std::shared_ptr<Message> getNextMessage();
    void processMessages();
    private:
    MessageManager() = default;
    ~MessageManager() = default;

    MessageManager(const MessageManager&) = delete;
    MessageManager& operator=(const MessageManager&) = delete;

    st::queue<std::shared_ptr<Message>> messageQueue;
};
#endif // !__MESSAGE_MANAGER_H_
