#pragma once
#ifndef __ABILITY_MESSAGE_H_
#define __ABILITY_MESSAGE_H_

#include <cstdint>

#include "abilities.h"
#include "message.h"

class AbilityMessage : public Message {
public:
    uint32_t eID;
    ShipAbilities ability;
    AbilityMessage(uint32_t eID, ShipAbilities ability) : eID(eID), ability(ability) {}
};

#endif // !__ABILITY_MESSAGE_H_
