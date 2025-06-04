#pragma once

#include <iostream>

#include "System.h"

class KeyboardMessage;
class PickupPickedMessage;
class LevelUpMessage;

class PlayerSystem : public System {
public:
    const int EXP_TO_LEVEL_UP = 30;
    PlayerSystem(EntityManager& eMgr);
    void update(EntityManager& eMgr, const double& dT) override;

    void updateMovement(uint32_t eID);
    void updateAbilities(uint32_t eID, double dT);
    void addAbility(uint32_t eID, WeaponAbilities ability);

private:
    void handleKeyboardInput(std::shared_ptr<KeyboardMessage> msg);
    void handlePickupPickedMessage(std::shared_ptr<PickupPickedMessage> msg);
    void handleLevelUpMessage(std::shared_ptr<LevelUpMessage> msg);
    void levelUpPassive(uint32_t player, size_t pasive, uint8_t level);
    EntityManager& eMgr;
};
