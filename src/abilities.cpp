#include "Components.h"

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
        text = weaponUpgradeTexts[index][level];
    } else if (choice.type == AbilityType::Passive) {
        bool isActive = player.ownedPassives[index];
        level = player.passiveLevels[index];
        level = isActive ? level + 1 : level;
        level = std::min(level, maxPassiveLevel[index]);
        text = passiveUpgradeTexts[index][level];
    } else if (choice.type == AbilityType::Money) {
        text = "+50 money";
    } else if (choice.type == AbilityType::Health) {
        text = "+50 health";
    }
    return text;
}
