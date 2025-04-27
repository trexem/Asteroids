#pragma once

#include "passiveAbilities.h"
#include "weaponAbilities.h"

#include <array>

enum class AbilityType {
    Weapon,
    Passive,
    Money,
    Health
};

struct AbilityChoice {
    AbilityType type;
    size_t index;
};

constexpr std::array<std::array<const char*, 10>, static_cast<size_t>(WeaponAbilities::WeaponAbilitiesCount)> weaponUpgradeTexts = {{
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
        "+5 Laser Damage",
        "FINAL: +1 Projectile, +10 Damage, -0.05s Cooldown"
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
        "Laser Beam",
        "+5% Beam size"
        "+5 Damage",                    // Level 2: Damage 10→15
        "+5% Beam size",               // Level 3: Size 1→1.05
        "+5 Damage",                    // Level 4: Damage 15→20
        "-0.10s Cooldown",              // Level 5: Cooldown 3.20→3.10
        "+5% Beam size",               // Level 6: Size 1.05→1.10
        "+5 Damage",                    // Level 7: Damage 20→25
        "+5% Beam size",               // Level 8: Size 1.10→1.15
        "FINAL: 20 Damage, +25% size, 3s Cooldown"
    },
    // Explosives
    {
        "Explosives",
        "+1 Explosive",                // Level 1: Projectile 1→2
        "+0 Damage",                    // Level 2: Damage stays 0
        "+1 Explosive",                 // Level 3: Projectile 2→3
        "+0 Damage",                    // Level 4: Damage stays 0
        "-0.10s Cooldown",              // Level 5: Cooldown 1.20→1.10
        "+1 Explosive",                 // Level 6: Projectile 3→4
        "+0 Damage",                    // Level 7: Damage stays 0
        "+0 Damage",                    // Level 8: Damage stays 0
        "FINAL: 5 Explosives, 1s Cooldown"
    },
    // GravitySaws
    {
        "Gravity Saws",
        "+1 Saw Blade",                // Level 1: Projectile 1→2
        "+5 Damage",                    // Level 2: Damage 10→15
        "+1 Saw Blade",                 // Level 3: Projectile 2→3
        "+5 Damage",                    // Level 4: Damage 15→20
        "-0.10s Cooldown",              // Level 5: Cooldown 3.20→3.10
        "+1 Saw Blade",                 // Level 6: Projectile 3→4
        "+5 Damage",                    // Level 7: Damage 20→25
        "+5 Damage",                    // Level 8: Damage 25→30
        "FINAL: 5 Blades, 40 Damage, 3.05s Cooldown"
    },
}};

constexpr std::array<std::array<const char*, 10>, static_cast<size_t>(PassiveAbilities::PassiveAbilitiesCount)> passiveUpgradeTexts = {{
    // PickupRange
    {
        "Pick up Range",
        "+50 Pick up Range",
        "+50 Pick up Range",
        "+50 Pick up Range",
        "+50 Pick up Range",
        "+100 Pick up Range",
        "+100 Pick up Range",
        "+100 Pick up Range",
        "+100 Pick up Range",
        "+200 Pick up Range"
    },
    
    // CooldownReduction
    {
        "Cooldown Reduction",
        "+1% Cooldown Reduction",
        "+4% Cooldown Reduction",
        "+5% Cooldown Reduction",
        "+5% Cooldown Reduction",
        "+5% Cooldown Reduction",
        "+5% Cooldown Reduction",
        "+5% Cooldown Reduction",
        "+5% Cooldown Reduction",
        "+10% Cooldown Reduction"
    },
    
    // Armor
    {
        "Armor",
        "+1% Damage Reduction",
        "+4% Damage Reduction",
        "+5% Damage Reduction",
        "+5% Damage Reduction",
        "+5% Damage Reduction",
        "+5% Damage Reduction",
        "+5% Damage Reduction",
        "+5% Damage Reduction",
        "+10% Damage Reduction"
    },
    
    // MaxHealth
    {
        "Max Health",
        "+5 Max Health",
        "+5 Max Health",
        "+10 Max Health",
        "+10 Max Health",
        "+10 Max Health",
        "+10 Max Health",
        "+10 Max Health",
        "+10 Max Health",
        "+20 Max Health"
    },
    
    // HealthRegen
    {
        "Health Regeneration",
        "+0.5 HP/s",
        "+0.5 HP/s",
        "+1 HP/s",
        "+1 HP/s",
        "+1 HP/s",
        "+1 HP/s",
        "+1 HP/s",
        "+1 HP/s",
        "+2 HP/s"
    },
    
    // MovementSpeed
    {
        "Movement Speed",
        "+1% Speed",
        "+4% Speed",
        "+5% Speed",
        "+5% Speed",
        "+5% Speed",
        "+5% Speed",
        "+5% Speed",
        "+5% Speed",
        "+10% Speed"
    },
    
    // ProjectileCount
    {
        "Projectile Count",
        "+1 Projectile",
        "+1 Projectile",
        "+1 Projectile",
        "+1 Projectile",
        "+1 Projectile",
        "", // Empty for levels beyond 5
        "",
        "",
        ""
    },
    
    // Size
    {
        "Projectile Size",
        "+1% Size",
        "+4% Size",
        "+5% Size",
        "+5% Size",
        "+5% Size",
        "", // Empty for levels beyond 5
        "",
        "",
        ""
    },
    
    // Damage
    {
        "Damage Multiplier",
        "+5% Damage",
        "+5% Damage",
        "+10% Damage",
        "+15% Damage",
        "+15% Damage",
        "+50% Damage", // Last level jump
        "",
        "",
        ""
    },
    
    // ProjectileSpeed
    {
        "Projectile Speed",
        "+1% Speed",
        "+4% Speed",
        "+5% Speed",
        "+5% Speed",
        "+5% Speed",
        "", // Empty for levels beyond 5
        "",
        "",
        ""
    }

}};
