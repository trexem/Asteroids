#pragma once

#include "json.hpp"
#include "MetaStats.hpp"

#include <fstream>
#include <map>
#include <string>
#include <vector>

namespace GameSave {
    using json = nlohmann::json;
    constexpr char XOR_KEY = 0xEF;

    inline std::string simpleObfuscate(const std::string& data) {
        std::string result = data;
        for (auto& c : result) c ^= XOR_KEY;
        return result;
    }

    struct MetaStats {
        int coins = 0;
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

    inline bool saveStatsToFile(const std::string& path, MetaStats& stats) {
        std::ofstream file(path, std::ios::binary);
        if (!file) return false;
        json j = stats.toJson();
        file << simpleObfuscate(j.dump());
        return true;
    }

    inline bool loadStatsFromFile(const std::string& path, MetaStats& stats) {
        std::ifstream file(path, std::ios::binary);
        if (!file) return false;
        std::string encrypted((std::istreambuf_iterator<char>(file)), {});
        try {
            stats.fromJson(json::parse(simpleObfuscate(encrypted)));
        } catch (...) { return false; }
        return true;
    }
}
