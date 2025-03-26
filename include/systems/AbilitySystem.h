#pragma once
#ifndef __ABILITY_SYSTEM_H_
#define __ABILITY_SYSTEM_H_

#include "abilities.h"
#include "AbilityMessage.h"
#include "MessageManager.h"
#include "System.h"
#include "utils.hpp"

class AbilitySystem : public System {
public:
    AbilitySystem(EntityManager* eManager);

private:
    void handleAbilityMessage(std::shared_ptr<AbilityMessage> msg);
    void spawnLaserGun(uint32_t eID);
    EntityManager* eManager;
};

#endif // !__ABILITY_SYSTEM_H_
