#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <random>
#include <vector>

#include "utils.hpp"

enum class PassiveAbilities : size_t {
    PickupRadius,
    CooldownReduction,
    Armor,
    MaxHealth,
    HealthRegen,
    MovementSpeed,
    ProjectileCount,
    Size,
    Damage,
    ProjectileSpeed,
    PassiveAbilitiesCount
};

// Single value per level (much more memory efficient)
constexpr float passiveValues[static_cast<size_t>(PassiveAbilities::PassiveAbilitiesCount)][10] = {
    // PickUpRadius
    { 200, 250, 300, 350, 400, 500, 600, 700, 800, 1000},

    // CooldownReduction (percentage)
    {0.01f, 0.05f, 0.10f, 0.15f, 0.20f, 0.25f, 0.30f, 0.35f, 0.40f, 0.50f},
    
    // Armor (damage reduction)
    {0.01f, 0.05f, 0.10f, 0.15f, 0.20f, 0.25f, 0.30f, 0.35f, 0.40f, 0.50f},
    
    // MaxHealth (flat bonus)
    {5.0f, 10.0f, 20.0f, 30.0f, 40.0f, 50.0f, 60.0f, 70.0f, 80.0f, 100.0f},
    
    // HealthRegen (per second)
    {0.5f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 10.0f},
    
    // MovementSpeed (percentage)
    {1.01f, 1.05f, 1.10f, 1.15f, 1.20f, 1.25f, 1.30f, 1.35f, 1.40f, 1.50f},

    //ProjectileCount
    {1, 2, 3, 4, 5},

    //Size (multiplier)
    {1.01f, 1.05f, 1.1f, 1.15f, 1.2f, 1.25f},

    //Damage (multiplier)
    {1.05f, 1.1f, 1.2f, 1.35f, 1.5f, 2.0f},

    //ProjectileSpeed (multiplier)
    {1.01f, 1.05f, 1.1f, 1.15f, 1.2f, 1.25f},
};

constexpr uint8_t maxPassiveLevel[static_cast<size_t>(PassiveAbilities::PassiveAbilitiesCount)] = {
    9,  // PickupRadius
    9,  // CooldownReduction
    9,  // Armor
    9,  // MaxHealth
    9,  // HealthRegen
    9,  // MovementSpeed
    4,  // ProjectileCount
    5,  // Size
    5,  // Damage
    9   // ProjectileSpeed
};

constexpr std::array<const char*, static_cast<size_t>(PassiveAbilities::PassiveAbilitiesCount)> passiveIconLocation = {
    "data/img/pickupRangeIcon.bmp",
    "data/img/cooldownReductionIcon.bmp",
    "data/img/armorIcon.bmp",
    "data/img/maxHealthIcon.bmp",
    "data/img/healthRegenIcon.bmp",
    "data/img/movementSpeedIcon.bmp",
    "data/img/projectileCountIcon.bmp",
    "data/img/sizeIcon.bmp",
    "data/img/damageIcon.bmp",
    "data/img/projectileSpeedIcon.bmp",
};
