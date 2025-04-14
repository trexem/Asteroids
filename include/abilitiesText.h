#pragma once

#include "abilities.h"

#include <array>
constexpr std::array<std::array<const char*, 10>, static_cast<size_t>(ShipAbilities::ShipAbilitiesCount)> abilityUpgradeTexts = {{
    // LaserGun
    {
        "Laser Gun",
        "+1 Laser Projectile",
        "+5 Laser Damage",
        "+1 Laser Projectile",
        "+5 Laser Damage",
        "-0.05s Cooldown",
        "+1 Laser Projectile",
        "+5 Laser Damage",
        "-0.05s Cooldown",
        "FINAL: +1 Projectile, +10 Damage"
    },
    // Rocket
    {
        "Rocket Launcher",
        "+1 Rocket",
        "+10 Splash Damage",
        "+1 Rocket",
        "+10 Splash Damage",
        "-0.05s Cooldown",
        "+1 Rocket",
        "+10 Splash Damage",
        "-0.05s Cooldown",
        "FINAL: +1 Rocket, +20 Damage"
    },
    // Laser
    {
        "Laser",
        "+1 Rocket",
        "+10 Splash Damage",
        "+1 Rocket",
        "+10 Splash Damage",
        "-0.05s Cooldown",
        "+1 Rocket",
        "+10 Splash Damage",
        "-0.05s Cooldown",
        "FINAL: +1 Rocket, +20 Damage"
    },
    // Explosives
    {
        "Explosives",
        "+1 Rocket",
        "+10 Splash Damage",
        "+1 Rocket",
        "+10 Splash Damage",
        "-0.05s Cooldown",
        "+1 Rocket",
        "+10 Splash Damage",
        "-0.05s Cooldown",
        "FINAL: +1 Rocket, +20 Damage"
    },
    // GravitySaws
    {
        "Gravity Saw",
        "+1 Rocket",
        "+10 Splash Damage",
        "+1 Rocket",
        "+10 Splash Damage",
        "-0.05s Cooldown",
        "+1 Rocket",
        "+10 Splash Damage",
        "-0.05s Cooldown",
        "FINAL: +1 Rocket, +20 Damage"
    },
    // PickupRange
    {
        "Pick up Range",
        "+1 Rocket",
        "+10 Splash Damage",
        "+1 Rocket",
        "+10 Splash Damage",
        "-0.05s Cooldown",
        "+1 Rocket",
        "+10 Splash Damage",
        "-0.05s Cooldown",
        "FINAL: +1 Rocket, +20 Damage"
    }
}};