#include "SettingsManager.h"
#include "VolumeMessage.h"
#include "GraphicsSettingsMessage.h"
#include "MessageManager.h"
#include "GUI.h"

#include <SDL3/SDL_log.h>

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
        SDL_Log("Failed to load settings. Defaulting settings.");
        settings = GameSettings();
    } else {
        SDL_Log("Successfully loaded settings.");
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
    if (!GameSave::saveSettings(settings)) {
        SDL_Log("Failed to save settings.");
    } else {
        SDL_Log("Settings Successfully saved.");
    }
    scale = {currentResolution->second.x / SCREEN_WIDTH, currentResolution->second.y / SCREEN_HEIGHT};
    SDL_Log("Scale: %f, %f", scale.x, scale.y);
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
    auto bestFit = resolutions.end();
    float minDistanceSq = std::numeric_limits<float>::max();

    for (auto it = resolutions.begin(); it != resolutions.end(); ++it) {
        const FPair& res = it->second;
        
        if (res.x <= size.x && res.y <= size.y) {
            if (bestFit == resolutions.end() || (res.x * res.y) > (bestFit->second.x * bestFit->second.y)) {
                bestFit = it;
            }
        }
    }
    if (bestFit == resolutions.end()) {
        bestFit = std::min_element(
            resolutions.begin(), resolutions.end(), [](const auto& a, const auto& b){
                return (a.second.x * a.second.y) < (b.second.x * b.second.y);
        });
        SDL_Log("No resolution fits. Using smallest available: %fx%f", bestFit->second.x, bestFit->second.y);
    } else {
        SDL_Log("Best fit inside screen is: %fx%f", bestFit->second.x, bestFit->second.y);
    }
    return bestFit;
}

void SettingsManager::updateResolution() {
    SDL_Log("Updating resolution. CurrentScreenSize: (%fx%f). CurrentResolution: (%fx%f).",
        currentScreenSize.x, currentScreenSize.y, currentResolution->second.x, currentResolution->second.y);
    MessageManager::instance().sendMessage(std::make_shared<GraphicsSettingsMessage>(
        settings.fullscreen, settings.vsync, currentScreenSize, currentResolution->second));
    currentScreenSize = currentResolution->second;
    settings.screenWidth = currentScreenSize.x;
    settings.screenHeight = currentScreenSize.y;
    save();
}

void SettingsManager::setWindowSize(const int& w, const int& h) {
    currentScreenSize = { static_cast<float>(w), static_cast<float>(h) };
    currentResolution = findResolutionBySize(currentScreenSize);
    SDL_Log("Setting window size. CurrentScreenSize: (%fx%f). CurrentResolution: (%fx%f).",
        currentScreenSize.x, currentScreenSize.y, currentResolution->second.x, currentResolution->second.y);
    settings.screenWidth = currentScreenSize.x;
    settings.screenHeight = currentScreenSize.y;
    if (currentScreenSize.x > currentResolution->second.x) {
        screenPos.x = (currentScreenSize.x - currentResolution->second.x) / 2.0f;
    }
    if (currentScreenSize.y > currentResolution->second.y) {
        screenPos.y = (currentScreenSize.y - currentResolution->second.y) / 2.0f;
    }
    SDL_Log("Screen pos updated to: %fx%f", screenPos.x, screenPos.y);
    save();
}
