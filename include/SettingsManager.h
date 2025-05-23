#pragma once

#include "GameSettings.h"
#include "VolumeSource.h"
#include "FPair.h"

using GameSettings = GameSave::GameSettings;

class SettingsManager {
public:
    static SettingsManager& instance();
    static constexpr float SCREEN_WIDTH = 1920;
    static constexpr float SCREEN_HEIGHT = 1080;
    FPair scale;
    FPair currentScreenSize;
    FPair screenPos;
    std::map<std::string, FPair>::iterator currentResolution;

    void load();
    void save();

    FPair scaleSize(FPair baseSize) const;
    FPair scalePos(FPair basePos, FPair prevScreenSize) const;
    void nextResolution();
    void backResolution();
    void increaseVolume(const VolumeSource& source);
    void decreaseVolume(const VolumeSource& source);
    int getVolume(const VolumeSource& source);
    GameSettings& get();
    std::string getCurrentResolutionName() const;
    std::map<std::string, FPair>::iterator findResolutionBySize(const FPair& size);
    void updateResolution();
    void setWindowSize(const int& w, const int& h);
private:
    GameSettings settings;
    SettingsManager();
    std::map<std::string, FPair> resolutions;
    
};
