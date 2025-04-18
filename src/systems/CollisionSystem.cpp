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

        int minCellX = static_cast<int>(trans->position.x) / CELL_SIZE;
        int minCellY = static_cast<int>(trans->position.y) / CELL_SIZE;
        int maxCellX = static_cast<int>(trans->position.x + col->width) / CELL_SIZE;
        int maxCellY = static_cast<int>(trans->position.y + col->height) / CELL_SIZE;

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
                    TypesSet::sameType(EntityType::Experience, typeA->type, typeB->type)) {
                    continue;
                }
                std::cout << "Checking Collision for: " << typeA->type << " and " << typeB->type << std::endl;
                if (checkCollision(transA, colA, transB, colB)) {
                    std::cout << "Collision detected" << std::endl;
                    if (TypesSet::match(TypesSet::PLAYER_EXPERIENCE, typeA->type, typeB->type)) {
                        std::cout << "Player Experience Collision detected" << std::endl;
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
    SDL_FRect rectA = {
        transA.position.x + colA.position.x,
        transA.position.y + colA.position.y,
        colA.width,
        colA.height
    };
    
    SDL_FRect rectB = {
        transB.position.x + colB.position.x,
        transB.position.y + colB.position.y,
        colB.width,
        colB.height
    };
    // Fast bounding sphere check first
    const float combinedRadius = colA.getBoundingRadius() + colB.getBoundingRadius();
    std::cout << "Combined radius: " << combinedRadius << std::endl;
    const float distSq = getSquaredDistanceBetweenCenters(rectA, rectB);
    std::cout << "distSq: " << distSq << std::endl;
    
    if (distSq > combinedRadius * combinedRadius) {
        return false;  // No collision possible
    }
    
    // Precise shape-specific checks
    if (colA.shape == Shape::Circle && 
        colB.shape == Shape::Circle) {
        // Circle-circle
        const float actualRadius = colA.radius + colB.radius;
        return distSq <= actualRadius * actualRadius;
    }
    else {
        if (colA.shape == Shape::Circle) {
            return checkCircleRect(colA, rectA, colB, rectB);
        }
        else if (colB.shape == Shape::Circle) {
            return checkCircleRect(colB, rectB, colA, rectA);
        }
        else {
            return SDL_HasRectIntersectionFloat(&rectA, &rectB);
        }
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
