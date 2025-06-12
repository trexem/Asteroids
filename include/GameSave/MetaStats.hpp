#pragma once

#include <string>
#include <unordered_map>

enum class ShipType {
    TANK,
    FREE_MOVE,
};

enum class UpgradeType {
    Speed,
    Damage,
    FireRate,
    ProjectileCount,
    Experience,
    GoldProb,
    GoldValue,
    PickupRange,
    HealthRegen,
    MaxHealth,
    Armor,
    UpgradesCount
};

constexpr float upgradesValues[static_cast<size_t>(UpgradeType::UpgradesCount)][11] {
    { 0.6f,    0.7f,   0.8f,    0.9f,   1.0f,    1.2f,  1.4f,  1.6f,  1.8f,   2.0f,  2.25f}, //Speed
    {  1.0f,  1.05f,   1.1f,   1.15f,   1.2f,   1.25f,  1.3f,  1.4f,  1.5f,  1.75f,   2.0f}, //Damage Multiplier
    {  0.0f,   0.05f,  0.1f,   0.15f,   0.2f,   0.25f}, //Fire rate Multiplier
    {  0.0f,   1.0f,   2.0f,    3.0f}, //Projectile Count
    {  0.0f, 0.025f,  0.05f,    0.1f,  0.15f,    0.3f,  0.5f,  1.0f,  1.5f,   2.5f,   5.0f}, //Experience
    {  1.0f,   2.0f,   3.0f,    4.0f,   5.0f,    6.0f,  7.0f,  8.0f,  9.0f,  10.0f,  15.0f}, //GoldProb
    {  0.0f,   0.1f,   0.2f,    0.3f,   0.5f,    0.8f,  1.0f,  1.5f,  2.0f,   2.5f,   5.0f}, //GoldValue
    { 50.0f, 100.0f, 150.0f,  200.0f, 250.0f, 350.0f}, //Collection Radius
    {  0.0f,  0.05f,   0.1f,    0.2f,   0.5f,   0.75f,  1.0f,  1.5f,  2.5f,   5.0f,  10.0f}, //Health Regen  base health per secondd
    {  1.0f,  1.25f,   1.5f,   1.75f,   2.0f,   2.25f,  2.5f,  3.0f,  3.5f,   4.0f,   5.0f}, //Max Health multiplier for SHIP_BASE_HEALTH = 100
    {  0.0f,  0.01f,  0.05f,   0.10f,  0.15f,   0.20f, 0.25f, 0.30f, 0.35f,   0.40f, 0.50f}, //Armor
};

constexpr int upgradesMaxLevel[static_cast<size_t>(UpgradeType::UpgradesCount)] {
    10,
    10,
    5,
    3,
    10,
    10,
    10,
    5,
    10,
    10,
    10,
};

constexpr int upgradesCost[static_cast<size_t>(UpgradeType::UpgradesCount)][10] {
    {  5,  10,   20,  30,   50, 100, 150, 200, 500, 1000}, //Speed
    {  5,  10,   20,  30,   50, 100, 150, 200, 500, 1000}, //Damage
    { 10,  30,  100, 200, 1000},                           //Fire rate
    { 20, 150, 1000},                                      //Projectile Count
    {  5,  10,   20,  30,   50, 100, 150, 200, 500, 1000}, //Experience
    {  5,  10,   20,  30,   50, 100, 150, 200, 500, 1000}, //GoldProb
    {  5,  10,   20,  30,   50, 100, 150, 200, 500, 1000}, //GoldValue
    { 10,  30,  100, 200, 1000},                           //Collection Radius
    {  5,  10,   20,  30,   50, 100, 150, 200, 500, 1000}, //Health Regen
    {  5,  10,   20,  30,   50, 100, 150, 200, 500, 1000}, //Max Health
    {  5,  10,   20,  30,   50, 100, 150, 200, 500, 1000}, //Armor
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
    {UpgradeType::Speed, "Speed"},
    {UpgradeType::Damage, "Damage"},
    {UpgradeType::FireRate, "FireRate"},
    {UpgradeType::ProjectileCount, "ProjectileCount"},
    {UpgradeType::Experience, "Experience"},
    {UpgradeType::GoldProb, "GoldProb"},
    {UpgradeType::GoldValue, "GoldValue"},
    {UpgradeType::PickupRange, "PickupRange"},
    {UpgradeType::HealthRegen, "HealthRegen"},
    {UpgradeType::MaxHealth, "MaxHealth"},
    {UpgradeType::Armor, "Armor"},
};

inline const std::unordered_map<UpgradeType, std::string> upgradeTextNames = {
    {UpgradeType::Speed, "speed"},
    {UpgradeType::Damage, "damage"},
    {UpgradeType::FireRate, "fireRate"},
    {UpgradeType::ProjectileCount, "projectileCount"},
    {UpgradeType::Experience, "experience"},
    {UpgradeType::GoldProb, "goldProb"},
    {UpgradeType::GoldValue, "goldValue"},
    {UpgradeType::PickupRange, "pickupRange"},
    {UpgradeType::HealthRegen, "healthRegen"},
    {UpgradeType::MaxHealth, "maxHealth"},
    {UpgradeType::Armor, "armor"},
};

inline const std::unordered_map<std::string, UpgradeType> upgradeTypeString = {
    {"Speed", UpgradeType::Speed},
    {"Damage", UpgradeType::Damage},
    {"FireRate", UpgradeType::FireRate},
    {"ProjectileCount", UpgradeType::ProjectileCount},
    {"Experience", UpgradeType::Experience},
    {"GoldProb", UpgradeType::GoldProb},
    {"GoldValue", UpgradeType::GoldValue},
    {"PickupRange", UpgradeType::PickupRange},
    {"HealthRegen", UpgradeType::HealthRegen},
    {"MaxHealth", UpgradeType::MaxHealth},
    {"Armor", UpgradeType::Armor},
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

inline std::string to_key(UpgradeType type) {
    auto it = upgradeTextNames.find(type);
    return it != upgradeTextNames.end() ? it->second : "UNKNOWN";
}

inline UpgradeType upgradeTypeFromString(const std::string& name) {
    auto it = upgradeTypeString.find(name);
    return it != upgradeTypeString.end() ? it->second : UpgradeType::Speed;
}
