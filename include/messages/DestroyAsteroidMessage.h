#pragma once

#include <cstdint>

#include "abilities.h"
#include "message.h"

class DestroyAsteroidMessage : public Message {
public:
    uint32_t id;
    DestroyAsteroidMessage(uint32_t id) : id(id) {}
};
