#include "CollisionSystem.h"
#include "utils.hpp"

#include <iostream>

void CollisionSystem::update(EntityManager* eManager) {
    auto physicsEntities = eManager->getEntitiesWithComponent(ComponentType::Physics);
    //Check collision between ship and asteroid
    for (int i = 0; i < physicsEntities.size(); i++) {
        auto entityA = physicsEntities.at(i);
        if (eManager->hasComponent<CollisionComponent>(entityA)) {
            const auto collisionA = eManager->getComponentData<CollisionComponent>(entityA);
            const auto transA = eManager->getComponentData<TransformComponent>(entityA);
            const auto typeA = eManager->getComponentData<TypeComponent>(entityA);
            SDL_FRect a = {transA.position.x, transA.position.y, collisionA.width, collisionA.height};
            for (int j = (i+1); j < physicsEntities.size(); j++) {
                auto entityB = physicsEntities.at(j);
                if (eManager->hasComponent<CollisionComponent>(entityB)) {
                    const auto typeB = eManager->getComponentData<TypeComponent>(entityB);
                    if ((typeA.type == EntityType::Shot && typeB.type == EntityType::Shot) ||
                        typeA.type == EntityType::Player && typeB.type == EntityType::Shot) {
                        continue;
                    }
                    const auto collisionB = eManager->getComponentData<CollisionComponent>(entityB);
                    const auto transB = eManager->getComponentData<TransformComponent>(entityB);
                    SDL_FRect b = {transB.position.x, transB.position.y, collisionB.width, collisionB.height};
                    if (checkCollision(a, b)) {
                        std::vector<uint32_t> ids;
                        ids.push_back(entityA);
                        ids.push_back(entityB);
                        auto msg = std::make_shared<CollisionMessage>(ids);
                        MessageManager::getInstance().sendMessage(msg);
                    }
                }
            }
        }
    }
}
