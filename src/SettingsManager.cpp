#include "SettingsManager.h"

SettingsManager::SettingsManager() {
    resolutions = {
        {"800x600",    {800, 600}},
        {"1280x720",   {1280, 720}},
        {"1600x900",   {1600, 900}},
        {"1920x1080",  {1920, 1080}},
        {"2560x1440",  {2560, 1440}},
        {"3840x2160",  {3840, 2160}},
    };
    currentResolution = resolutions.find("1920x1080");
    if (currentResolution == resolutions.end())
        currentResolution = resolutions.begin();
}

SettingsManager& SettingsManager::instance() {
    static SettingsManager instance;
    return instance;
}

void SettingsManager::load() {
    if (!GameSave::loadSettings(settings)) {
        settings = GameSettings();
        currentScreenSize = {static_cast<float>(settings.screenWidth), static_cast<float>(settings.screenHeight)};
        scale = {settings.screenWidth / SCREEN_WIDTH, settings.screenHeight / SCREEN_HEIGHT};
    }
}

void SettingsManager::save() {
    GameSave::saveSettings(settings);
    scale = {settings.screenWidth / SCREEN_WIDTH, settings.screenHeight / SCREEN_HEIGHT};
    currentScreenSize = {static_cast<float>(settings.screenWidth), static_cast<float>(settings.screenHeight)};
}

GameSettings& SettingsManager::get() { return settings; }

FPair SettingsManager::scaleSize(FPair baseSize) const {
    return {baseSize.x * scale.x, baseSize.y * scale.y};
}

FPair SettingsManager::scalePos(FPair basePos, FPair prevScreenSize) const {
    FPair pos = basePos / prevScreenSize * currentScreenSize;
    return pos;
}

void SettingsManager::nextResolution() {
    ++currentResolution;
    if (currentResolution == resolutions.end()) {
        currentResolution = resolutions.begin();
    }
}

void SettingsManager::backResolution() {
    if (currentResolution == resolutions.begin()) {
        currentResolution = resolutions.end();
    }
    --currentResolution;
}

std::string SettingsManager::getCurrentResolutionName() const {
    return currentResolution->first;
}
