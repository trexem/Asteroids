#pragma once

#include "System.h"
#include "utils.hpp"

class FollowSystem : public System {
public:
    void update(EntityManager* eManager);
};
