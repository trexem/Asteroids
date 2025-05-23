#pragma once

#include "json.hpp"
#include "MetaStats.hpp"

#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_iostream.h>
#include <SDL3/SDL_log.h>

namespace GameSave {
    using json = nlohmann::json;
    constexpr char XOR_KEY = 0xEF;

    inline std::string simpleObfuscate(const std::string& data) {
        std::string result = data;
        for (auto& c : result) c ^= XOR_KEY;
        return result;
    }

    struct MetaStats {
        uint32_t coins = 0;
        std::map<UpgradeType, int> upgrades;
        std::vector<ShipType> unlockedShips;
        json toJson() {
            json j;
            j["coins"] = coins;
            
            json upgradesJson;
            for (auto& [k, v] : upgrades) {
                upgradesJson[to_string(k)] = v;
            }
            j["upgrades"] = upgradesJson;
            
            json shipsJson = json::array();
            for (auto& ship : unlockedShips) {
                shipsJson.push_back(to_string(ship));
            }
            j["unlockedShips"] = shipsJson;

            return j;
        }

        void fromJson(const json& j) {
            j.at("coins").get_to(coins);

            upgrades.clear();
            for (auto& [k, v] : j["upgrades"].items()) {
                upgrades[upgradeTypeFromString(k)] = v;
            }

            unlockedShips.clear();
            for (auto& v : j["unlockedShips"]) {
                unlockedShips.push_back(shipTypeFromString(v));
            }
        }
    };

    inline bool saveStatsToFile(MetaStats& stats) {
#if defined (__ANDROID__)
        char* prefPathCStr = SDL_GetPrefPath("trexem", "WeNeedMoreAsteroids");
        if (!prefPathCStr) {
            SDL_Log("SDL_GetPrefPath failed in saveSettings.");
            return false;
        }
        std::string basePath = prefPathCStr;
        SDL_free(prefPathCStr);
        std::string path = basePath + "stats.json";
#else
        std::string path = "data/stats.json";
#endif
        SDL_IOStream* io = SDL_IOFromFile(path.c_str(), "wb");
        if (!io) return false;
        json j = stats.toJson();
        std::string obfuscated = simpleObfuscate(j.dump());
        if (SDL_WriteIO(io, obfuscated.data(), obfuscated.size()) != obfuscated.size()) {
            SDL_CloseIO(io);
            return false;
        }

        SDL_CloseIO(io);
        return true;
    }

    inline bool loadStatsFromFile(MetaStats& stats) {
#if defined (__ANDROID__)
        char* prefPathCStr = SDL_GetPrefPath("trexem", "WeNeedMoreAsteroids");
        if (!prefPathCStr) {
            SDL_Log("SDL_GetPrefPath failed in saveSettings.");
            return false;
        }
        std::string basePath = prefPathCStr;
        SDL_free(prefPathCStr);
        std::string path = basePath + "stats.json";
#else
        std::string path = "data/stats.json";
#endif
        SDL_IOStream* io = SDL_IOFromFile(path.c_str(), "rb");
        if (!io) return false;
        Sint64 size = SDL_GetIOSize(io);
        if (size <= 0) {
            SDL_CloseIO(io);
            return false;
        }
        std::string encrypted(size, '\0');
        Sint64 read = SDL_ReadIO(io, encrypted.data(), size);
        SDL_CloseIO(io);

        if (read != size) return false;
        SDL_Log("Raw content loaded from stats.json: [%s]", simpleObfuscate(encrypted.substr(0, read)).c_str());
        try {
            stats.fromJson(json::parse(simpleObfuscate(encrypted)));
        } catch (...) { return false; }
        return true;
    }
}
