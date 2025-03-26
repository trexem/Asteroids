#pragma once
#ifndef __PHYSICS_SYSTEM_H_
#define __PHYSICS_SYSTEM_H_

#include "System.h"
#include "MessageManager.h"
#include "EntityManager.h"

class PhysicsSystem : public System {
public:
    void update(EntityManager* eManager);
};

#endif // !__PHYSICS_SYSTEM_H_