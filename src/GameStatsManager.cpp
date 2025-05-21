#include "GameStatsManager.h"

#include <SDL3/SDL_log.h>

GameStatsManager& GameStatsManager::instance()  {
    static GameStatsManager instance;
    return instance;
}

void GameStatsManager::load(const std::string& path) {
    if (!GameSave::loadStatsFromFile(path, stats)) {
        SDL_Log("Failed to load stats, defaulting.");
        stats = MetaStats();
    }
}

void GameStatsManager::save(const std::string& path) {
    if (!GameSave::saveStatsToFile(path, stats)) {
        SDL_Log("Failed to save stats to %s", path.c_str());
    }
}

MetaStats& GameStatsManager::getStats() { return stats; }

void GameStatsManager::addCoins(int amount) { stats.coins += amount; }

bool GameStatsManager::spendCoins(int amount) {
    if (stats.coins >= amount) {
        stats.coins -= amount;
        return true;
    }
    return false;
}

int GameStatsManager::getUpgradeLevel(UpgradeType type) const {
    auto it = stats.upgrades.find(type);
    return (it != stats.upgrades.end()) ? it->second : 0;
}

float GameStatsManager::getUpgradeValue(UpgradeType type) const {
    int lvl = getUpgradeLevel(type);
    return upgradesValues[static_cast<size_t>(type)][lvl];
}
