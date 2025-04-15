#pragma once

#include "message.h"
#include <vector>
#include <cstdint>

class AsteroidAsteroidCollisionMessage : public Message {
public:
    std::vector<uint32_t> entityID;
    AsteroidAsteroidCollisionMessage(std::vector<uint32_t> entityID) : entityID(entityID) {}
};
