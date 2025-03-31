#pragma once
#ifndef __DAMAGE_SYSTEM_H_
#define __DAMAGE_SYSTEM_H_

#include "CollisionMessage.h"
#include "MessageManager.h"
#include "System.h"

class DamageSystem : public System {
public:
    DamageSystem(EntityManager* eManager);
private:
    void handleCollisionMessage(std::shared_ptr<CollisionMessage> msg);
    void handleAsteroidShotCollision(uint32_t shot, uint32_t asteroid);
    EntityManager* eManager;
};

#endif // !__DAMAGE_SYSTEM_H_
