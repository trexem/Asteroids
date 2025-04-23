#pragma once

#include "AnimationMessage.h"
#include "CollisionMessage.h"
#include "DestroyAsteroidMessage.h"
#include "ExplodeMessage.h"
#include "ExperienceSpawnMessage.h"
#include "MessageManager.h"
#include "System.h"

#include <unordered_set>

class DamageSystem : public System {
public:
    DamageSystem(EntityManager* eManager);
    void update(double dT);
private:
    void handleCollisionMessage(std::shared_ptr<CollisionMessage> msg);
    void handleAsteroidShotCollision(uint32_t shot, uint32_t asteroid);
    void handleAsteroidShipCollision(uint32_t ship, uint32_t asteroid);
    void handleAsteroidExplosionCollision(uint32_t explosion, uint32_t asteroid);
    void handleAsteroidSawCollision(uint32_t saw, uint32_t asteroid);
    void handleAsteroidLaserCollision(uint32_t laser, uint32_t asteroid);
    void handleAsteroidExplosiveCollision(uint32_t explosive, uint32_t asteroid);
    EntityManager* eManager;
    const double EXPLOSION_COOLDOWN = 0.25f;
    const double LASER_COOLDOWN = 0.10f;
};
