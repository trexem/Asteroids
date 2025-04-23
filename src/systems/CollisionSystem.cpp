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
                if (TypesSet::sameType(EntityType::Shot, typeA->type, typeB->type) || 
                    TypesSet::match(TypesSet::PLAYER_SHOT, typeA->type, typeB->type) ||
                    TypesSet::sameType(EntityType::Experience, typeA->type, typeB->type) ||
                    TypesSet::match(TypesSet::SAW_PLAYER, typeA->type, typeB->type) ||
                    TypesSet::match(TypesSet::LASER_PLAYER, typeA->type, typeB->type) ||
                    TypesSet::match(TypesSet::ROCKET_PLAYER, typeA->type, typeB->type) ||
                    TypesSet::match(TypesSet::EXPLOSIVE_PLAYER, typeA->type, typeB->type)) {
                    continue;
                }
                std::cout << "Checking Collision for: " << typeA->type << " and " << typeB->type << std::endl;
                if (checkCollision(transA, colA, transB, colB)) {
                    //std::cout << "Collision detected" << std::endl;
                    if (TypesSet::match(TypesSet::PLAYER_EXPERIENCE, typeA->type, typeB->type)) {
                        //std::cout << "Player Experience Collision detected" << std::endl;
                        ExperienceComponent* xp = eManager->getComponentDataPtr<ExperienceComponent>(b);
                        auto msg = std::make_shared<ExperiencePickupMessage>(a, xp->xp);
                        eManager->destroyEntityLater(b);
                        MessageManager::getInstance().sendMessage(msg);
                        continue;
                    } else if (TypesSet::sameType(EntityType::Asteroid, typeA->type, typeB->type)) {
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
    // Convert both to SDL_FRect for SDL's collision check
    const SDL_FRect rectA = {
        transA.position.x + colA.position.x,
        transA.position.y + colA.position.y,
        colA.width,
        colA.height
    };
    
    const SDL_FRect rectB = {
        transB.position.x + colB.position.x,
        transB.position.y + colB.position.y,
        colB.width,
        colB.height
    };
    // std::cout << "Combined radius: " << combinedRadius << std::endl;
    const float distSq = getSquaredDistanceBetweenCenters(rectA, rectB);
    // std::cout << "distSq: " << distSq << std::endl;
    
    // Precise shape-specific checks
    if (colA.shape == Shape::Circle && 
        colB.shape == Shape::Circle) {
        // Circle-circle
        const float actualRadius = colA.radius + colB.radius;
        return distSq <= actualRadius * actualRadius;
    }
    else if (colA.shape == Shape::Circle || colB.shape == Shape::Circle) {
        if (colA.shape == Shape::Circle) {
            return checkCircleOBB(colA, transA, colB, transB);
        }
        return checkCircleOBB(colB, transB, colA, transB);
    } else {
        auto cornersA = getCorners(transA, colA);
        auto cornersB = getCorners(transB, colB);
        return checkOBBCollision(cornersA, cornersB);
    }
}

bool CollisionSystem::checkCircleRect(
    const CollisionComponent& circle, const SDL_FRect& circleRect,
    const CollisionComponent& rect, const SDL_FRect& rectRect
) {
    // Find closest point on rectangle to circle center
    float closestX = std::clamp(circleRect.x, rectRect.x, rectRect.x + rectRect.w);
    float closestY = std::clamp(circleRect.y, rectRect.y, rectRect.y + rectRect.h);

    // Calculate distance
    float dx = circleRect.x - closestX;
    float dy = circleRect.y - closestY;
    float distSq = dx*dx + dy*dy;

    return distSq <= circle.radius * circle.radius;
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
    
    // Find closest point on OBB to circle center
    FPair closest = circleCenter;
    
    // For each edge of OBB, project circle center onto edge
    float minDistSq = INFINITY;
    
    // Check against each edge of OBB
    for (int i = 0; i < 4; i++) {
        int j = (i + 1) % 4;
        FPair edge = {obbCorners[j].x - obbCorners[i].x, obbCorners[j].y - obbCorners[i].y};
        float edgeLengthSq = edge.x * edge.x + edge.y * edge.y;
        
        if (edgeLengthSq == 0.0f) continue;
        
        // Project circle center onto edge
        FPair toCenter = {circleCenter.x - obbCorners[i].x, circleCenter.y - obbCorners[i].y};
        float projection = (toCenter.x * edge.x + toCenter.y * edge.y) / edgeLengthSq;
        projection = std::clamp(projection, 0.0f, 1.0f);
        
        FPair pointOnEdge = {
            obbCorners[i].x + projection * edge.x,
            obbCorners[i].y + projection * edge.y
        };
        
        float distSq = (pointOnEdge.x - circleCenter.x) * (pointOnEdge.x - circleCenter.x) +
                       (pointOnEdge.y - circleCenter.y) * (pointOnEdge.y - circleCenter.y);
        
        if (distSq < minDistSq) {
            minDistSq = distSq;
            closest = pointOnEdge;
        }
    }
    
    // Check distance to closest point
    float distSq = (closest.x - circleCenter.x) * (closest.x - circleCenter.x) +
                   (closest.y - circleCenter.y) * (closest.y - circleCenter.y);
    
    return distSq <= (circle.radius * circle.radius);
}
