#pragma once

#include <cstdint>

#include "abilities.h"
#include "message.h"

class ExplodeMessage : public Message {
public:
    uint32_t id;
    ExplodeMessage(uint32_t id) : id(id) {}
};
