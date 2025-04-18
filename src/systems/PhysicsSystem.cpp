#include "PhysicsSystem.h"
#include <iostream>

void PhysicsSystem::update(EntityManager* eManager, double dT) {
    auto entities = eManager->getEntitiesWithComponent(ComponentType::Physics);
    for (uint32_t e : entities) {
        PhysicsComponent pComp = eManager->getComponentData<PhysicsComponent>(e);
        if (pComp.acceleration != 0) {
            pComp.velocity += pComp.acceleration * dT;
            eManager->setComponentData(e, pComp);
        }
    }
}
