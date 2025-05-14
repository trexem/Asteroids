#pragma once

#include "GameSettings.h"
#include "FPair.h"

using GameSettings = GameSave::GameSettings;

class SettingsManager {
public:
    static SettingsManager& instance();
    static constexpr float SCREEN_WIDTH = 1920;
    static constexpr float SCREEN_HEIGHT = 1080;
    FPair scale;
    FPair currentScreenSize;

    void load();
    void save();

    FPair scaleSize(FPair baseSize) const;
    FPair scalePos(FPair basePos, FPair prevScreenSize) const;
    void nextResolution();
    void backResolution();
    GameSettings& get();
    std::string getCurrentResolutionName() const;
private:
    GameSettings settings;
    SettingsManager();
    std::map<std::string, FPair> resolutions;
    std::map<std::string, FPair>::iterator currentResolution;
};
