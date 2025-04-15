#include "Components.h"

std::vector<ShipAbilities> getRandomAbilityChoices(const PlayerComponent& player) {
    std::vector<ShipAbilities> availableAbilities;

    for (size_t i = 0; i < static_cast<size_t>(ShipAbilities::ShipAbilitiesCount); ++i) {
        if (player.abilityLevels[i] < 9) {
            availableAbilities.push_back(static_cast<ShipAbilities>(i));
        }
    }

    std::shuffle(availableAbilities.begin(), availableAbilities.end(), std::mt19937{std::random_device{}()});

    // Return up to 3
    size_t count = std::min<size_t>(3, availableAbilities.size());
    return std::vector<ShipAbilities>(availableAbilities.begin(), availableAbilities.begin() + count);
}

std::string getNextUpgradeText(const PlayerComponent& player, ShipAbilities ability) {
    size_t index = static_cast<size_t>(ability);
    size_t level = player.abilityLevels[index];
    bool isActive = player.abilities[index];
    level = isActive ? level + 1 : level;
    // Clamp to max level
    level = std::min(level, size_t(9));

    return abilityUpgradeTexts[index][level];
}
