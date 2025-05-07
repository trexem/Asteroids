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
    FireRate,
    PROJECTILE_COUNT,
    EXPERIENCE,
    GoldProb,
    GoldValue,
    COLLECTION_RADIUS,
    HEALTH_REGEN,
    MAX_HEALTH,
    ARMOR,
    UpgradesCount
};

constexpr float upgradesValues[static_cast<size_t>(UpgradeType::UpgradesCount)][10] {
    {}, //Speed
    {}, //Damage
    {}, //Fire rate
    {}, //Projectile Count
    { 0.0f, 0.025f, 0.05f,   0.1f, 0.15f,  0.3f,  0.5f,  1.0f,  1.5f,   2.5f}, //Experience
    { 1.0f,   2.0f,  3.0f,   4.0f,  5.0f,  6.0f,  7.0f,  8.0f,  9.0f,  10.0f}, //GoldProb
    { 0.0f, 0.025f, 0.05f, 0.075f,  0.1f, 0.13f, 0.16f,  0.2f, 0.25f,   0.5f}, //GoldValue
    {}, //Collection Radius
    {}, //Health Regen
    {}, //Max Health
    {}, //Armor
};

constexpr int upgradesCost[static_cast<size_t>(UpgradeType::UpgradesCount)][10] {
    { 5, 10, 20, 30, 50, 100, 150, 200, 500, 1000}, //Speed
    { 5, 10, 20, 30, 50, 100, 150, 200, 500, 1000}, //Damage
    { 5, 10, 20, 30, 50, 100, 150, 200, 500, 1000}, //Fire rate
    { 5, 10, 20, 30, 50, 100, 150, 200, 500, 1000}, //Projectile Count
    { 5, 10, 20, 30, 50, 100, 150, 200, 500, 1000}, //Experience
    { 5, 10, 20, 30, 50, 100, 150, 200, 500, 1000}, //GoldProb
    { 5, 10, 20, 30, 50, 100, 150, 200, 500, 1000}, //GoldValue
    { 5, 10, 20, 30, 50, 100, 150, 200, 500, 1000}, //Collection Radius
    { 5, 10, 20, 30, 50, 100, 150, 200, 500, 1000}, //Health Regen
    { 5, 10, 20, 30, 50, 100, 150, 200, 500, 1000}, //Max Health
    { 5, 10, 20, 30, 50, 100, 150, 200, 500, 1000}, //Armor
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
    {UpgradeType::FireRate, "FireRate"},
    {UpgradeType::PROJECTILE_COUNT, "PROJECTILE_COUNT"},
    {UpgradeType::EXPERIENCE, "EXPERIENCE"},
    {UpgradeType::GoldProb, "GoldProb"},
    {UpgradeType::GoldValue, "GoldValue"},
    {UpgradeType::COLLECTION_RADIUS, "COLLECTION_RADIUS"},
    {UpgradeType::HEALTH_REGEN, "HEALTH_REGEN"},
    {UpgradeType::MAX_HEALTH, "MAX_HEALTH"},
    {UpgradeType::ARMOR, "ARMOR"},
};

inline const std::unordered_map<std::string, UpgradeType> upgradeTypeString = {
    {"SPEED", UpgradeType::SPEED},
    {"DAMAGE", UpgradeType::DAMAGE},
    {"FireRate", UpgradeType::FireRate},
    {"PROJECTILE_COUNT", UpgradeType::PROJECTILE_COUNT},
    {"EXPERIENCE", UpgradeType::EXPERIENCE},
    {"GoldProb", UpgradeType::GoldProb},
    {"GoldValue", UpgradeType::GoldValue},
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
