#include "SettingsManager.h"
#include "VolumeMessage.h"
#include "GraphicsSettingsMessage.h"
#include "MessageManager.h"

SettingsManager::SettingsManager() {
    resolutions = {
        {"800x600",    {800, 600}},
        {"1280x720",   {1280, 720}},
        {"1600x900",   {1600, 900}},
        {"1920x1080",  {1920, 1080}},
        {"2560x1440",  {2560, 1440}},
        {"3840x2160",  {3840, 2160}},
    };
    load();
}

SettingsManager& SettingsManager::instance() {
    static SettingsManager instance;
    return instance;
}

void SettingsManager::load() {
    if (!GameSave::loadSettings(settings)) {
        settings = GameSettings();
    }
    FPair loadedSize {static_cast<float>(settings.screenWidth), static_cast<float>(settings.screenHeight)};
    currentResolution = findResolutionBySize(loadedSize);
    if (currentResolution == resolutions.end())
        currentResolution = resolutions.begin();

    currentScreenSize = loadedSize;
    scale = {
        loadedSize.x / SCREEN_WIDTH,
        loadedSize.y / SCREEN_HEIGHT
    };
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
    updateResolution();
}

void SettingsManager::backResolution() {
    if (currentResolution == resolutions.begin()) {
        currentResolution = resolutions.end();
    }
    --currentResolution;
    updateResolution();
}

std::string SettingsManager::getCurrentResolutionName() const {
    return currentResolution->first;
}

void SettingsManager::increaseVolume(const VolumeSource& source) {
    switch (source) {
    case VolumeSource::MasterVolume:
        settings.masterVolume++;
        if (settings.masterVolume > 10) settings.masterVolume = 10;
        MessageManager::instance()
            .sendMessage(std::make_shared<VolumeMessage>(source, settings.masterVolume));
        break;
    case VolumeSource::MusicVolume:
        settings.musicVolume++;
        if (settings.musicVolume > 10) settings.musicVolume = 10;
        MessageManager::instance()
            .sendMessage(std::make_shared<VolumeMessage>(source, settings.musicVolume));
        break;
    case VolumeSource::SFXVolume:
        settings.sfxVolume++;
        if (settings.sfxVolume > 10) settings.sfxVolume = 10;
        MessageManager::instance()
            .sendMessage(std::make_shared<VolumeMessage>(source, settings.sfxVolume));
        break;
    }
    save();
}

void SettingsManager::decreaseVolume(const VolumeSource& source) {
    switch (source) {
    case VolumeSource::MasterVolume:
        settings.masterVolume--;
        if (settings.masterVolume < 0) settings.masterVolume = 0;
        MessageManager::instance()
            .sendMessage(std::make_shared<VolumeMessage>(source, settings.masterVolume));
        break;
    case VolumeSource::MusicVolume:
        settings.musicVolume--;
        if (settings.musicVolume < 0) settings.musicVolume = 0;
        MessageManager::instance()
            .sendMessage(std::make_shared<VolumeMessage>(source, settings.musicVolume));
        break;
    case VolumeSource::SFXVolume:
        settings.sfxVolume--;
        if (settings.sfxVolume < 0) settings.sfxVolume = 0;
        MessageManager::instance()
            .sendMessage(std::make_shared<VolumeMessage>(source, settings.sfxVolume));
        break;
    }
    save();
}

int SettingsManager::getVolume(const VolumeSource& source) {
    return source == VolumeSource::MasterVolume ? settings.masterVolume :
        source == VolumeSource::MusicVolume ? settings.musicVolume :
        settings.sfxVolume;
}

std::map<std::string, FPair>::iterator SettingsManager::findResolutionBySize(const FPair& size) {
    for (auto it = resolutions.begin(); it != resolutions.end(); ++it) {
        if (it->second == size) return it;
    }
    return resolutions.end();
}

void SettingsManager::updateResolution() {
    std::cout << currentScreenSize << " & " << currentResolution->second << std::endl;
    MessageManager::instance().sendMessage(std::make_shared<GraphicsSettingsMessage>(
        settings.fullscreen, settings.vsync, currentScreenSize, currentResolution->second));
    currentScreenSize = currentResolution->second;
    settings.screenWidth = currentScreenSize.x;
    settings.screenHeight = currentScreenSize.y;
    save();
}
