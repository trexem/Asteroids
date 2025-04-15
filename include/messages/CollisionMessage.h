#pragma once

#include "message.h"
#include <vector>
#include <cstdint>

class CollisionMessage : public Message {
public:
    std::vector<uint32_t> entityID;
    CollisionMessage(std::vector<uint32_t> entityID) : entityID(entityID) {}
};
