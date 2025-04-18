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
    bool checkCollision(const TransformComponent& transA, const CollisionComponent& colA,
                        const TransformComponent& transB, const CollisionComponent& colB);
    bool checkCircleRect(const CollisionComponent& circle, const SDL_FRect& circleRect,
        const CollisionComponent& rect, const SDL_FRect& rectRect);
};
