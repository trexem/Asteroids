#pragma once

#include <cstdint>

#include "message.h"

class ExperiencePickupMessage : public Message {
public:
    uint32_t playerID;
    unsigned int amount;
    ExperiencePickupMessage(uint32_t player, unsigned int am) : playerID(player), amount(am) {}
};
