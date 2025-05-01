#include "CollisionSystem.h"
#include "utils.hpp"

#include <iostream>

void CollisionSystem::update(EntityManager* eManager) {
    std::unordered_map<std::pair<int, int>, std::vector<uint32_t>, pair_hash> spatialGrid;
    std::vector<uint32_t> physicsEntities = eManager->getEntitiesWithComponent(ComponentType::Physics);
    // std::cout << "PhysicsEntities count: " << physicsEntities.size() << std::endl;
    for (uint32_t entity : physicsEntities) {
        TransformComponent* trans = eManager->getComponentDataPtr<TransformComponent>(entity);
        CollisionComponent* col = eManager->getComponentDataPtr<CollisionComponent>(entity);
        if (!trans || !col) continue;

        auto corners = getCorners(*trans, *col);

        // Find the AABB that encloses the rotated OBB
        float minX = std::min({corners[0].x, corners[1].x, corners[2].x, corners[3].x});
        float maxX = std::max({corners[0].x, corners[1].x, corners[2].x, corners[3].x});
        float minY = std::min({corners[0].y, corners[1].y, corners[2].y, corners[3].y});
        float maxY = std::max({corners[0].y, corners[1].y, corners[2].y, corners[3].y});

        int minCellX = static_cast<int>(minX) / CELL_SIZE;
        int minCellY = static_cast<int>(minY) / CELL_SIZE;
        int maxCellX = static_cast<int>(maxX) / CELL_SIZE;
        int maxCellY = static_cast<int>(maxY) / CELL_SIZE;

        for (int x = minCellX; x <= maxCellX; ++x) {
            for (int y = minCellY; y <= maxCellY; ++y) {
                // std::cout << "adding " << entity << " to grid: " << x << ", " << y << std::endl;
                spatialGrid[{x, y}].push_back(entity);
            }
        }
    }
    // std::cout << "SpatialGrid count: " << spatialGrid.size() << std::endl;
    for (auto& cell : spatialGrid) {
        auto& entities = cell.second;

        for (size_t i = 0; i < entities.size(); ++i) {
            uint32_t a = entities[i];
            if (eManager->isDestroyed(a) || eManager->isMarkedForDestruction(a)) continue;
            TransformComponent transA = eManager->getComponentData<TransformComponent>(a);
            CollisionComponent colA = eManager->getComponentData<CollisionComponent>(a);
            TypeComponent* typeA = eManager->getComponentDataPtr<TypeComponent>(a);
            if (!typeA) continue;

            for (size_t j = i + 1; j < entities.size(); ++j) {
                uint32_t b = entities[j];
                if (eManager->isDestroyed(b) || eManager->isMarkedForDestruction(b)) continue;
                TransformComponent transB = eManager->getComponentData<TransformComponent>(b);
                CollisionComponent colB = eManager->getComponentData<CollisionComponent>(b);
                TypeComponent* typeB = eManager->getComponentDataPtr<TypeComponent>(b);
                if (!typeB) continue;
                EntityType tA = typeA->type;
                EntityType tB = typeB->type;
                if (TypesSet::shouldIgnoreCollision(tA, tB)) {
                    continue;
                }
                // std::cout << "Checking Collision for: " << tA << " and " << tB << std::endl;
                if (checkCollision(transA, colA, transB, colB)) {
                    // std::cout << "Collision detected" << std::endl;
                    if (TypesSet::match(TypesSet::PLAYER_EXPERIENCE, tA, tB)) {
                        //std::cout << "Player Experience Collision detected" << std::endl;
                        PickupComponent* xp = eManager->getComponentDataPtr<PickupComponent>(b);
                        auto msg = std::make_shared<ExperiencePickupMessage>(a, xp->value);
                        eManager->destroyEntityLater(b);
                        MessageManager::getInstance().sendMessage(msg);
                        continue;
                    } else if (TypesSet::sameType(EntityType::Asteroid, tA, tB)) {
                        std::vector<uint32_t> ids;
                        ids.push_back(a);
                        ids.push_back(b);
                        auto msg = std::make_shared<AsteroidAsteroidCollisionMessage>(ids);
                        MessageManager::getInstance().sendMessage(msg);
                        continue;
                    }
                    std::vector<uint32_t> ids;
                    ids.push_back(a);
                    ids.push_back(b);
                    auto msg = std::make_shared<CollisionMessage>(ids);
                    MessageManager::getInstance().sendMessage(msg);
                }
            }
        }
    }
}

bool CollisionSystem::checkCollision(
    const TransformComponent& transA, const CollisionComponent& colA,
    const TransformComponent& transB, const CollisionComponent& colB
) {
    // Precise shape-specific checks
    if (colA.shape == Shape::Circle && 
        colB.shape == Shape::Circle) {
        // Circle-circle
        float distSq = getSquaredDistanceBetweenCircles(transA.position, colA.radius, transB.position, colB.radius);
        const float combinedRadius  = colA.radius + colB.radius;
        return distSq <= combinedRadius  * combinedRadius ;
    }
    else if (colA.shape == Shape::Circle || colB.shape == Shape::Circle) {
        if (colA.shape == Shape::Circle) {
            return checkCircleOBB(colA, transA, colB, transB);
        }
        return checkCircleOBB(colB, transB, colA, transA);
    } else {
        auto cornersA = getCorners(transA, colA);
        auto cornersB = getCorners(transB, colB);
        return checkOBBCollision(cornersA, cornersB);
    }
}

bool CollisionSystem::checkOBBCollision(
    const std::array<FPair, 4>& cornersA,
    const std::array<FPair, 4>& cornersB
) {
    std::array<FPair, 4> axes;
    // Axes from box A
    axes[0] = {cornersA[1].x - cornersA[0].x, cornersA[1].y - cornersA[0].y};
    axes[1] = {cornersA[3].x - cornersA[0].x, cornersA[3].y - cornersA[0].y};
    // Axes from box B
    axes[2] = {cornersB[1].x - cornersB[0].x, cornersB[1].y - cornersB[0].y};
    axes[3] = {cornersB[3].x - cornersB[0].x, cornersB[3].y - cornersB[0].y};
    // Normalize all axes
    for (auto& axis : axes) {
        axis.normalize();
    }
    // Test each axis
    for (const auto& axis : axes) {
        // Project all points of A onto axis
        float minA = INFINITY, maxA = -INFINITY;
        for (const auto& corner : cornersA) {
            float projection = corner.x * axis.x + corner.y * axis.y;
            minA = std::min(minA, projection);
            maxA = std::max(maxA, projection);
        }
        
        // Project all points of B onto axis
        float minB = INFINITY, maxB = -INFINITY;
        for (const auto& corner : cornersB) {
            float projection = corner.x * axis.x + corner.y * axis.y;
            minB = std::min(minB, projection);
            maxB = std::max(maxB, projection);
        }
        
        // Check for overlap
        if (maxA < minB || maxB < minA) {
            return false; // Found a separating axis
        }
    }
    
    return true; // No separating axis found
}

bool CollisionSystem::checkCircleOBB(
    const CollisionComponent& circle, const TransformComponent& circleTrans,
    const CollisionComponent& obb, const TransformComponent& obbTrans
) {
    // Get OBB corners
    auto obbCorners = getCorners(obbTrans, obb);
    
    // Circle center in world space
    FPair circleCenter = {
        circleTrans.position.x + circle.position.x + circle.radius,
        circleTrans.position.y + circle.position.y + circle.radius
    };
    
    // For each edge of OBB, project circle center onto edge
    float minDistSq = INFINITY;

    auto squaredDistance = [](const FPair& a, const FPair& b) -> float {
        float dx = a.x - b.x;
        float dy = a.y - b.y;
        return dx * dx + dy * dy;
    };

    auto closestPointOnEdge = [](const FPair& a, const FPair& b, const FPair& point) -> FPair {
        FPair ab = {b.x - a.x, b.y - a.y};
        float abLengthSq = ab.x * ab.x + ab.y * ab.y;
        if (abLengthSq == 0.0f) return a;

        FPair ap = {point.x - a.x, point.y - a.y};
        float projection = (ap.x * ab.x + ap.y * ab.y) / abLengthSq;
        projection = std::clamp(projection, 0.0f, 1.0f);

        return {
            a.x + projection * ab.x,
            a.y + projection * ab.y
        };
    };
    
    // Check against each edge of OBB
    for (int i = 0; i < 4; i++) {
        int j = (i + 1) % 4;
        FPair pointOnEdge = closestPointOnEdge(obbCorners[i], obbCorners[j], circleCenter);
        float distSq = squaredDistance(circleCenter, pointOnEdge);
        if (distSq < minDistSq) {
            minDistSq = distSq;
        }
    }
    
    
    return minDistSq <= (circle.radius * circle.radius);
}
