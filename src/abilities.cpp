#include "Components.h"
#include "GameStatsManager.h"
#include "TextManager.h"

std::vector<AbilityChoice> getRandomAbilityChoices(const PlayerComponent& player) {
    std::vector<AbilityChoice> availableAbilities;

    auto addAbilityOptions = [&](AbilityType type, const auto& ownedArray, uint8_t ownedCount,
        const auto& levelsArray, const auto& maxLevels) {
        for (size_t i = 0; i < ownedArray.size(); i++) {
            if (ownedArray[i] && levelsArray[i] < maxLevels[i]) {
                availableAbilities.push_back({type, i});
            } else if (!ownedArray[i] && ownedCount < 3) {
                availableAbilities.push_back({type, i});
            }
        }
    };

    addAbilityOptions(
        AbilityType::Weapon,
        player.ownedWeapons,
        player.ownedWeaponsCount,
        player.weaponLevels,
        maxWeaponLevel
    );

    addAbilityOptions(
        AbilityType::Passive,
        player.ownedPassives,
        player.ownedPassivesCount,
        player.passiveLevels,
        maxPassiveLevel
    );

    std::shuffle(availableAbilities.begin(), availableAbilities.end(), std::mt19937{std::random_device{}()});

    // Return up to 3
    size_t count = std::min<size_t>(3, availableAbilities.size());
    return std::vector<AbilityChoice>(availableAbilities.begin(), availableAbilities.begin() + count);
}

std::string getNextUpgradeText(const PlayerComponent& player, const AbilityChoice& choice) {
    const size_t index = choice.index;
    uint8_t level;
    std::string text;

    if (choice.type == AbilityType::Weapon) {
        bool isActive = player.ownedWeapons[index];
        level = player.weaponLevels[index];
        level = isActive ? level + 1 : level;
        level = std::min(level, maxWeaponLevel[index]);
        std::string id = weaponId[index];
        float p1 = abilitiesCooldowns[index][level];
        float p2 = index == static_cast<size_t>(WeaponAbilities::Laser) ? abilitiesSize[index][level] * 100.0f
            : abilitiesProjectileCount[index][level];
        float p3 = abilitiesDamage[index][level];
        float p4 = index == static_cast<size_t>(WeaponAbilities::LaserGun) || index == static_cast<size_t>(WeaponAbilities::Rocket) 
            ? abilitiesProjectileSpeed[index][level] : static_cast<float>(abilitiesLifeTime[index][level]);
        if (index == static_cast<size_t>(WeaponAbilities::TeslaGun)) {
            p4 = abilitiesSize[index][level];
        }
        text = isActive ? TextManager::instance().format("abilityLvlUp." + id, p1, p2, p3, p4) :
            TextManager::instance().get("abilityDesc." + id);
    } else if (choice.type == AbilityType::Passive) {
        bool isActive = player.ownedPassives[index];
        level = player.passiveLevels[index];
        level = isActive ? level + 1 : level;
        level = std::min(level, maxPassiveLevel[index]);
        std::string id = passiveId[index];
        float value = passiveValues[index][level];
        if (static_cast<size_t>(PassiveAbilities::CooldownReduction) || 
            index == static_cast<size_t>(PassiveAbilities::Armor)) {
            value *= 100.0f;
        }
        text = isActive ? TextManager::instance().format("abilityLvlUp." + id, value) :
            TextManager::instance().get("abilityDesc." + id);
    } else if (choice.type == AbilityType::Money) {
        float goldMult = (1.0f + GameStatsManager::instance().getUpgradeValue(UpgradeType::GoldValue));
        int gold = 50 * goldMult;
        text = TextManager::instance().format("abilityDesc.money", gold);
    } else if (choice.type == AbilityType::Health) {
        text = TextManager::instance().get("abilityDesc.health");
    }
    return text;
}

std::string getAbilityTitle(const AbilityChoice& choice) {
    const size_t index = choice.index;
    std::string id;
    if (choice.type == AbilityType::Passive) {
        id = passiveId[index];
    } else if (choice.type == AbilityType::Weapon) {
        id = weaponId[index];
    } else if (choice.type == AbilityType::Money) {
        id = "money";
    } else {
        id = "health";
    }
    return TextManager::instance().get("abilityTitle." + id);
}
