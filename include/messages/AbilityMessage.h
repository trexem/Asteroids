#pragma once

#include <cstdint>

#include "abilities.h"
#include "message.h"

class AbilityMessage : public Message {
public:
    uint32_t eID;
    ShipAbilities ability;
    AbilityMessage(uint32_t eID, ShipAbilities ability) : eID(eID), ability(ability) {}
};
