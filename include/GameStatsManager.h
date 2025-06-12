#pragma once

#include "GameSave.hpp"

using MetaStats = GameSave::MetaStats;

class GameStatsManager {
public:
    static GameStatsManager& instance();

    void load();

    void save();

    MetaStats& getStats();

    void addCoins(int amount);
    bool spendCoins(int amount);
    void setNewMax(uint32_t kills, uint32_t level, uint32_t time);

    int getUpgradeLevel(UpgradeType type) const;
    float getUpgradeValue(UpgradeType type) const;

private:
    MetaStats stats;
    GameStatsManager() = default;
};
