#pragma once

#include "AsteroidAsteroidCollisionMessage.h"
#include "CollisionMessage.h"
#include "PickupPickedMessage.h"
#include "MessageManager.h"
#include "System.h"

#include <unordered_set>

class CollisionSystem : public System {
    const int CELL_SIZE = 100;
public:
    void update(EntityManager* eManager);
    bool checkCollision(const TransformComponent& transA, const CollisionComponent& colA,
                        const TransformComponent& transB, const CollisionComponent& colB);
    bool checkOBBCollision(
        const std::array<FPair, 4>& cornersA,
        const std::array<FPair, 4>& cornersB
    );
    bool checkCircleOBB(
        const CollisionComponent& circle, const TransformComponent& circleTrans,
        const CollisionComponent& obb, const TransformComponent& obbTrans
    );
};
