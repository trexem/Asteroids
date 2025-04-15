#pragma once

#include "AsteroidAsteroidCollisionMessage.h"
#include "CollisionMessage.h"
#include "ExperiencePickupMessage.h"
#include "MessageManager.h"
#include "System.h"

#include <unordered_set>

class CollisionSystem : public System {
    const int CELL_SIZE = 100;
public:
    void update(EntityManager* eManager);
};
