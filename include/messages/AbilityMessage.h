#pragma once

#include <cstdint>

#include "weaponAbilities.h"
#include "message.h"

class AbilityMessage : public Message {
public:
    uint32_t eID;
    WeaponAbilities ability;
    AbilityMessage(uint32_t eID, WeaponAbilities ability) : eID(eID), ability(ability) {}
};
