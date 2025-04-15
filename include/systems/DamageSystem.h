#pragma once

#include "AnimationMessage.h"
#include "CollisionMessage.h"
#include "DestroyAsteroidMessage.h"
#include "ExperienceSpawnMessage.h"
#include "MessageManager.h"
#include "System.h"

#include <unordered_set>

class DamageSystem : public System {
public:
    DamageSystem(EntityManager* eManager);
private:
    void handleCollisionMessage(std::shared_ptr<CollisionMessage> msg);
    void handleAsteroidShotCollision(uint32_t shot, uint32_t asteroid);
    void handleAsteroidShipCollision(uint32_t ship, uint32_t asteroid);
    EntityManager* eManager;
};
