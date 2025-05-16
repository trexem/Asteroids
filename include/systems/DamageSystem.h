#pragma once

#include "AnimationMessage.h"
#include "CollisionMessage.h"
#include "DestroyAsteroidMessage.h"
#include "ExplodeMessage.h"
#include "PickupsSpawnMessage.h"
#include "GameStateManager.h"
#include "MessageManager.h"
#include "System.h"

#include <unordered_set>

struct DamageContext {
    uint32_t source;
    uint32_t target;
    float damage;
    EntityType sourceType;
    EntityType targetType;
    double cooldownDuration;
    bool destroySource;
    bool destroyTarget;
    Animation animation;
};

class DamageSystem : public System {
public:
    DamageSystem(EntityManager& eM);
    void update(EntityManager& eMgr, const double& dT) override;
private:
    void applyDamage(const DamageContext& ctx);
    void handleCollisionMessage(std::shared_ptr<CollisionMessage> msg);
    void handleDamageToAsteroid(uint32_t asteroid, uint32_t damageSource);
    void handleAsteroidShotCollision(uint32_t shot, uint32_t asteroid);
    void handleAsteroidRocketCollision(uint32_t rocket, uint32_t asteroid);
    void handleAsteroidShipCollision(uint32_t ship, uint32_t asteroid);
    void handleAsteroidExplosionCollision(uint32_t explosion, uint32_t asteroid);
    void handleAsteroidSawCollision(uint32_t saw, uint32_t asteroid);
    void handleAsteroidLaserCollision(uint32_t laser, uint32_t asteroid);
    void handleAsteroidExplosiveCollision(uint32_t explosive, uint32_t asteroid);
    void cleanupDeadEntities();
    EntityManager& eMgr;
    std::unordered_map<uint32_t, std::unordered_map<uint32_t, float>> entitiesDamageCooldown;
    static constexpr double DAMAGE_COOLDOWN = 0.2;
};
