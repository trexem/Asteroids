#pragma once

#include <iostream>

#include "AbilityMessage.h"
#include "EntityManager.h"
#include "PickupPickedMessage.h"
#include "GameStateManager.h"
#include "GameStatsManager.h"
#include "KeyboardMessage.h"
#include "LevelUpMessage.h"
#include "MessageManager.h"
#include "System.h"

class PlayerSystem : public System {
public:
    PlayerSystem(EntityManager* eManager);
    void update(double dT);

    void updateMovement(uint32_t eID);
    void updateAbilities(uint32_t eID, double dT);
    void addAbility(uint32_t eID, WeaponAbilities ability);

private:
    void handleKeyboardInput(std::shared_ptr<KeyboardMessage> msg);
    void handlePickupPickedMessage(std::shared_ptr<PickupPickedMessage> msg);
    void handleLevelUpMessage(std::shared_ptr<LevelUpMessage> msg);
    void levelUpPassive(uint32_t player, size_t pasive, uint8_t level);
    EntityManager* eManager;
};
