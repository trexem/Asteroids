#pragma once

#include "json.hpp"

#include <fstream>
#include <string>
#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_iostream.h>
#include "Log.h"


namespace GameSave {
    using json = nlohmann::json;

    struct GameSettings {
        int screenWidth = 1920;
        int screenHeight = 1080;
        bool fullscreen = false;
        bool vsync = true;

        int masterVolume = 10;
        int musicVolume = 10;
        int sfxVolume = 10;

        json toJson() const {
            return {
                {"screenWidth", screenWidth},
                {"screenHeight", screenHeight},
                {"fullscreen", fullscreen},
                {"vsync", vsync},
                {"masterVolume", masterVolume},
                {"musicVolume", musicVolume},
                {"sfxVolume", sfxVolume},
            };
        }

        void fromJson(const json& j) {
            j.at("screenWidth").get_to(screenWidth);
            j.at("screenHeight").get_to(screenHeight);
            j.at("fullscreen").get_to(fullscreen);
            j.at("vsync").get_to(vsync);
            j.at("masterVolume").get_to(masterVolume);
            j.at("musicVolume").get_to(musicVolume);
            j.at("sfxVolume").get_to(sfxVolume);
        }
    };

    inline bool saveSettings(const GameSettings& settings) {
#if defined (__ANDROID__)
        char* prefPathCStr = SDL_GetPrefPath("trexem", "WeNeedMoreAsteroids");
        if (!prefPathCStr) {
            DEBUG_LOG("SDL_GetPrefPath failed in saveSettings.");
            return false;
        }
        std::string basePath = prefPathCStr;
        SDL_free(prefPathCStr);
        std::string path = basePath + "settings.json";
#else
        std::string path = "settings.json";
#endif
        DEBUG_LOG("Settings save path: %s", path.c_str());
        SDL_IOStream* io = SDL_IOFromFile(path.c_str(), "wb");
        if (!io) return false;
        std::string text = settings.toJson().dump(4);
        DEBUG_LOG("json to save settings: %s", text.c_str());
        if (SDL_WriteIO(io, text.data(), text.size()) != text.size()) {
            SDL_CloseIO(io);
            return false;
        }
        SDL_CloseIO(io);
        return true;
    }

    inline bool loadSettings(GameSettings& settings) {
#if defined (__ANDROID__)
        char* prefPathCStr = SDL_GetPrefPath("trexem", "WeNeedMoreAsteroids");
        if (!prefPathCStr) {
            DEBUG_LOG("SDL_GetPrefPath failed in saveSettings.");
            return false;
        }
        std::string basePath = prefPathCStr;
        SDL_free(prefPathCStr);
        std::string path = basePath + "settings.json";
#else
        std::string path = "settings.json";
#endif
        DEBUG_LOG("Settings load path: %s", path.c_str());
        SDL_IOStream* io = SDL_IOFromFile(path.c_str(), "rb");
        if (!io) {
            DEBUG_LOG("Failed to open settings file for reading: %s", SDL_GetError());
            return false;
        }

        Sint64 size = SDL_GetIOSize(io);
        DEBUG_LOG("Reported size of settings file: %lld", (long long)size);
        if (size <= 0) {
            DEBUG_LOG("Settings file is empty or size error.");
            SDL_CloseIO(io);
            return false;
        }

        std::string text(size, '\0');
        Sint64 read = SDL_ReadIO(io, text.data(), size);
        DEBUG_LOG("Bytes read from settings file: %lld", (long long)read);
        if (read <= 0) {
            DEBUG_LOG("Error reading settings file or zero bytes read. SDL_GetError: %s", SDL_GetError());
            SDL_CloseIO(io);
            return false;
        }
        if (read < size) {
            DEBUG_LOG("Partial read from settings file. Expected %lld, got %lld", (long long)size, (long long)read);
        }
        SDL_CloseIO(io);
        DEBUG_LOG("Raw content loaded from settings.json: [%s]", text.substr(0, read).c_str());
        try {
            settings.fromJson(json::parse(text));
        } catch (...) { return false; }
        return true;
    }
}
