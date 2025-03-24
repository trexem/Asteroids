#pragma once
#ifndef __PHYSICS_SYSTEM_H_
#define __PHYSICS_SYSTEM_H_

#include "System.h"
#include "MessageManager.h"
#include "ApplyForceMessage.h"
#include "EntityManager.h"

class PhysicsSystem : public System {
public:
    PhysicsSystem(EntityManager* eManager);

private:
    void handleApplyForceMessage(std::shared_ptr<ApplyForceMessage> msg);
    EntityManager* eManager;
};

#endif // !__PHYSICS_SYSTEM_H_