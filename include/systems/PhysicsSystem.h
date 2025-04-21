#pragma once

#include "System.h"
#include "MessageManager.h"
#include "EntityManager.h"

class PhysicsSystem : public System {
public:
    void update(EntityManager* eManager, double dT);
};
