#pragma once

#include <string>
#include <unordered_map>

enum class ShipType {
    TANK,
    FREE_MOVE,
};

enum class UpgradeType {
    SPEED,
    DAMAGE,
    FIRE_RATE,
    PROJECTILE_COUNT,
    EXPERIENCE,
    GOLD,
    COLLECTION_RADIUS,
    HEALTH_REGEN,
    MAX_HEALTH,
    ARMOR,
};

inline const std::unordered_map<ShipType, std::string> shipTypeNames = {
    {ShipType::TANK, "TANK"},
    {ShipType::FREE_MOVE, "FREE_MOVE"}
};

inline const std::unordered_map<std::string, ShipType> shipTypeString = {
    {"TANK", ShipType::TANK},
    {"FREE_MOVE", ShipType::FREE_MOVE}
};

inline const std::unordered_map<UpgradeType, std::string> upgradeTypeNames = {
    {UpgradeType::SPEED, "SPEED"},
    {UpgradeType::DAMAGE, "DAMAGE"},
    {UpgradeType::FIRE_RATE, "FIRE_RATE"},
    {UpgradeType::PROJECTILE_COUNT, "PROJECTILE_COUNT"},
    {UpgradeType::EXPERIENCE, "EXPERIENCE"},
    {UpgradeType::GOLD, "GOLD"},
    {UpgradeType::COLLECTION_RADIUS, "COLLECTION_RADIUS"},
    {UpgradeType::HEALTH_REGEN, "HEALTH_REGEN"},
    {UpgradeType::MAX_HEALTH, "MAX_HEALTH"},
    {UpgradeType::ARMOR, "ARMOR"},
};

inline const std::unordered_map<std::string, UpgradeType> upgradeTypeString = {
    {"SPEED", UpgradeType::SPEED},
    {"DAMAGE", UpgradeType::DAMAGE},
    {"FIRE_RATE", UpgradeType::FIRE_RATE},
    {"PROJECTILE_COUNT", UpgradeType::PROJECTILE_COUNT},
    {"EXPERIENCE", UpgradeType::EXPERIENCE},
    {"GOLD", UpgradeType::GOLD},
    {"COLLECTION_RADIUS", UpgradeType::COLLECTION_RADIUS},
    {"HEALTH_REGEN", UpgradeType::HEALTH_REGEN},
    {"MAX_HEALTH", UpgradeType::MAX_HEALTH},
    {"ARMOR", UpgradeType::ARMOR},
};

inline std::string to_string(ShipType type) {
    auto it = shipTypeNames.find(type);
    return it != shipTypeNames.end() ? it->second : "UNKNOWN";
}

inline ShipType shipTypeFromString(const std::string& name) {
    auto it = shipTypeString.find(name);
    return it != shipTypeString.end() ? it->second : ShipType::TANK; // or throw
}

inline std::string to_string(UpgradeType type) {
    auto it = upgradeTypeNames.find(type);
    return it != upgradeTypeNames.end() ? it->second : "UNKNOWN";
}

inline UpgradeType upgradeTypeFromString(const std::string& name) {
    auto it = upgradeTypeString.find(name);
    return it != upgradeTypeString.end() ? it->second : UpgradeType::SPEED;
}
