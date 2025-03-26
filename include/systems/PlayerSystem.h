#pragma once
#ifndef __PLAYER_SYSTEM_H_
#define __PLAYER_SYSTEM_H_

#include <iostream>

#include "AbilityMessage.h"
#include "EntityManager.h"
#include "KeyboardMessage.h"
#include "MessageManager.h"
#include "System.h"

class PlayerSystem : public System {
public:
    PlayerSystem(EntityManager* eManager);
    void update(double dT);

    void updateMovement(uint32_t eID);
    void updateAbilities(uint32_t eID, double dT);
    void addAbility(uint32_t eID, ShipAbilities ability);

private:
    void handleKeyboardInput(std::shared_ptr<KeyboardMessage> msg);
    EntityManager* eManager;
};

#endif // !__PLAYER_SYSTEM_H_