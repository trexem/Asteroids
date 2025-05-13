#pragma once

#include "GameSettings.h"

using GameSettings = GameSave::GameSettings;

class SettingsManager {
public:
    static SettingsManager& instance();

    void load();
    void save();
    GameSettings& get();
private:
    GameSettings settings;
    SettingsManager() = default;
};
