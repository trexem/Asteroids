#pragma once

#include "GameSave.hpp"

using MetaStats = GameSave::MetaStats;

class GameStatsManager {
public:
    static GameStatsManager& instance();

    void load(const std::string& path);

    void save(const std::string& path);

    MetaStats& getStats();

    void addCoins(int amount);
    bool spendCoins(int amount);

    int getUpgradeLevel(UpgradeType type) const;
    float getUpgradeValue(UpgradeType type) const;

private:
    MetaStats stats;
    GameStatsManager() = default;
};
