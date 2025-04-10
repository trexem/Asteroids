#pragma once

#include "CollisionMessage.h"
#include "ExperiencePickupMessage.h"
#include "MessageManager.h"
#include "System.h"

#include <unordered_set>

class CollisionSystem : public System {
public:
    void update(EntityManager* eManager);
};
