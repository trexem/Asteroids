#include "SettingsManager.h"

SettingsManager& SettingsManager::instance() {
    static SettingsManager instance;
    return instance;
}

void SettingsManager::load() {
    if (!GameSave::loadSettings(settings)) {
        settings = GameSettings();
    }
}

void SettingsManager::save() {
    GameSave::saveSettings(settings);
}

GameSettings& SettingsManager::get() { return settings; }
