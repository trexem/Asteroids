#pragma once

#include <cstdint>

#include "EntityType.h"
#include "message.h"

class PickupPickedMessage : public Message {
public:
    uint32_t playerID;
    unsigned int amount;
    EntityType type;
    PickupPickedMessage(uint32_t player, unsigned int am, EntityType t) : 
        playerID(player), amount(am), type(t) {}
};
