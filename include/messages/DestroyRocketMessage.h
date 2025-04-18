#pragma once

#include <cstdint>

#include "abilities.h"
#include "message.h"

class DestroyRocketMessage : public Message {
public:
    uint32_t id;
    DestroyRocketMessage(uint32_t id) : id(id) {}
};
