#pragma once

#include <cstdint>

#include "weaponAbilities.h"
#include "message.h"

class ExplodeMessage : public Message {
public:
    uint32_t id;
    WeaponAbilities source;
    ExplodeMessage(uint32_t id, WeaponAbilities s) : id(id), source(s) {}
};
