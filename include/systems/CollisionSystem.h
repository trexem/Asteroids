#pragma once
#ifndef __COLLISION_SYSTEM_H_
#define __COLLISION_SYSTEM_H_

#include "System.h"
#include "MessageManager.h"
#include "CollisionMessage.h"

class CollisionSystem : public System {
public:
    void update(EntityManager* eManager);
};

#endif // !__COLLISION_SYSTEM_H_
