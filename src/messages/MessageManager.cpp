#include "MessageManager.h"
#include <iostream>

MessageManager& MessageManager::getInstance() {
    static MessageManager instance;
    return instance;
}
