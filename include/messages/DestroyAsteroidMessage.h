#pragma once

#include <cstdint>

#include "weaponAbilities.h"
#include "message.h"

class DestroyAsteroidMessage : public Message {
public:
    uint32_t id;
    DestroyAsteroidMessage(uint32_t id) : id(id) {}
};
