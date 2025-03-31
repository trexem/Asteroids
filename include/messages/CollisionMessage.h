#pragma once
#ifndef __COLLISION_MESSAGE_H_
#define __COLLISION_MESSAGE_H_

#include "message.h"
#include <vector>
#include <cstdint>

class CollisionMessage : public Message {
public:
    std::vector<uint32_t> entityID;
    CollisionMessage(std::vector<uint32_t> entityID) : entityID(entityID) {}
};

#endif // !__COLLISION_MESSAGE_H_