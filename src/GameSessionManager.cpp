#include "GameSessionManager.h"

GameSessionManager& GameSessionManager::instance() {
    static GameSessionManager instance;
    return instance;
}

void GameSessionManager::reset() {
    stats.reset();
}

GameSessionStats& GameSessionManager::getStats() {
    return stats;
}
