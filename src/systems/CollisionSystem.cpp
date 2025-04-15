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
            TransformComponent* transA = eManager->getComponentDataPtr<TransformComponent>(a);
            CollisionComponent* colA = eManager->getComponentDataPtr<CollisionComponent>(a);
            TypeComponent* typeA = eManager->getComponentDataPtr<TypeComponent>(a);
            if (!transA || !colA || !typeA) continue;

            SDL_FRect rectA = {transA->position.x, transA->position.y, colA->width, colA->height};
            for (size_t j = i + 1; j < entities.size(); ++j) {
                uint32_t b = entities[j];
                if (eManager->isDestroyed(b) || eManager->isMarkedForDestruction(b)) continue;
                TransformComponent* transB = eManager->getComponentDataPtr<TransformComponent>(b);
                CollisionComponent* colB = eManager->getComponentDataPtr<CollisionComponent>(b);
                TypeComponent* typeB = eManager->getComponentDataPtr<TypeComponent>(b);
                if (!transB || !colB || !typeB) continue;
                if (TypesSet::sameType(EntityType::Shot, typeA->type, typeB->type) || 
                    TypesSet::match(TypesSet::PLAYER_SHOT, typeA->type, typeB->type) ||
                    TypesSet::sameType(EntityType::Experience, typeA->type, typeB->type)) {
                    continue;
                }
                SDL_FRect rectB = {transB->position.x, transB->position.y, colB->width, colB->height};
                if (checkCollision(rectA, rectB)) {
                    if (TypesSet::match(TypesSet::PLAYER_EXPERIENCE, typeA->type, typeB->type)) {
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
