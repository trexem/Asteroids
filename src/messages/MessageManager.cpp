#include "MessageManager.h"
#include <iostream>

MessageManager& MessageManager::instance() {
    static MessageManager instance;
    return instance;
}
