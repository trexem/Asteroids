#pragma once

#include "json.hpp"

#include <fstream>
#include <string>

namespace GameSave {
    using json = nlohmann::json;

    struct GameSettings {
        int screenWidth = 1920;
        int screenHeight = 1080;
        bool fullscreen = false;
        bool vsync = true;

        int masterVolume = 128;
        int musicVolume = 128;
        float sfxVolume = 128;

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
        std::ofstream file("data/settings.json");
        if (!file.is_open()) return false;
        file << settings.toJson().dump(4);
        return true;
    }

    inline bool loadSettings(GameSettings& settings) {
        std::ifstream file("data/settings.json");
        if (!file.is_open()) return false;

        json j;
        file >> j;
        settings.fromJson(j);
        return true;
    }
}
