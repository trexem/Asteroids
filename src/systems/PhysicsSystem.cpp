#include "PhysicsSystem.h"
#include "GameStateManager.h"
#include <iostream>

void PhysicsSystem::update(EntityManager& eMgr, const double& dT) {
    if (GameStateManager::instance().getState() == GameState::Playing) {
        auto entities = eMgr.getEntitiesWithComponent(ComponentType::Physics);
        for (uint32_t e : entities) {
            PhysicsComponent pComp = eMgr.getComponentData<PhysicsComponent>(e);
            if (pComp.acceleration != 0) {
                pComp.velocity += pComp.acceleration * dT;
                eMgr.setComponentData(e, pComp);
            }
        }
    }
}
