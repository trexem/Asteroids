#pragma once

#include <iostream>

#include "AbilityMessage.h"
#include "EntityManager.h"
#include "ExperiencePickupMessage.h"
#include "GameStateManager.h"
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
    void addAbility(uint32_t eID, ShipAbilities ability);

private:
    void handleKeyboardInput(std::shared_ptr<KeyboardMessage> msg);
    void handleExperiencePickupMessage(std::shared_ptr<ExperiencePickupMessage> msg);
    void handleLevelUpMessage(std::shared_ptr<LevelUpMessage> msg);
    EntityManager* eManager;
};
