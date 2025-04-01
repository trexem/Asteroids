#pragma once
#ifndef __DAMAGE_SYSTEM_H_
#define __DAMAGE_SYSTEM_H_

#include "AnimationMessage.h"
#include "CollisionMessage.h"
#include "MessageManager.h"
#include "System.h"

#include <unordered_set>

class DamageSystem : public System {
public:
    const std::unordered_set<EntityType> SHOT_ASTEROID = {EntityType::Shot, EntityType::Asteroid};
    const std::unordered_set<EntityType> PLAYER_ASTEROID = {EntityType::Player, EntityType::Asteroid};
    DamageSystem(EntityManager* eManager);
private:
    void handleCollisionMessage(std::shared_ptr<CollisionMessage> msg);
    void handleAsteroidShotCollision(uint32_t shot, uint32_t asteroid);
    void handleAsteroidShipCollision(uint32_t ship, uint32_t asteroid);
    EntityManager* eManager;
};

#endif // !__DAMAGE_SYSTEM_H_
