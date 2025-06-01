#include "GameStatsManager.h"

#include "Log.h"

GameStatsManager& GameStatsManager::instance()  {
    static GameStatsManager instance;
    return instance;
}

void GameStatsManager::load() {
    if (!GameSave::loadStatsFromFile(stats)) {
        DEBUG_LOG("Failed to load stats, defaulting.");
        stats = MetaStats();
    }
}

void GameStatsManager::save() {
    if (!GameSave::saveStatsToFile(stats)) {
        DEBUG_LOG("Failed to save stats.");
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
