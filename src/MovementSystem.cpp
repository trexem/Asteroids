#include "MovementSystem.h"
#include "utils.hpp"
#include <cmath>
#include <iostream>

void MovementSystem::update(EntityManager* eMgr, double dT) {
    for (uint32_t eID : eMgr->getEntitiesWithComponent(ComponentType::Physics)) {
        //std::cout << "Updating movement of component: " << eID << std::endl;
        if (eMgr->hasComponent<PlayerComponent>(eID)) {
            updatePlayer(eMgr, dT, eID);
        } else {
            PhysicsComponent physComp = eMgr->getComponentData<PhysicsComponent>(eID);
            StatsComponent statsComp = eMgr->getComponentData<StatsComponent>(eID);
            TransformComponent transComp = eMgr->getComponentData<TransformComponent>(eID);
            transComp.position += physComp.velocity * dT;
            eMgr->setComponentData<TransformComponent>(eID, transComp);
        }
    }
}

void MovementSystem::updatePlayer(EntityManager* eMgr, double dT, uint32_t eID) {
    PhysicsComponent physComp = eMgr->getComponentData<PhysicsComponent>(eID);
    StatsComponent statsComp = eMgr->getComponentData<StatsComponent>(eID);
    TransformComponent transComp = eMgr->getComponentData<TransformComponent>(eID);
    PlayerComponent playerComp = eMgr->getComponentData<PlayerComponent>(eID);
    if (playerComp.shipType) {
        double radians = transComp.rotDegrees * PI / 180;
        transComp.position.x += physComp.velocity.y * dT * sin(radians);
        transComp.position.y -= physComp.velocity.y * dT * cos(radians);
        transComp.rotDegrees += physComp.rotVelocity * dT;
        if (transComp.rotDegrees > 180) {
            transComp.rotDegrees -= 360;
        } else if (transComp.rotDegrees < -180) {
            transComp.rotDegrees += 360;
        }
    } else {
        transComp.position += physComp.velocity * dT;
    }
    // physComp.velocity.x *= 0.95;
    if (std::abs(physComp.velocity.x) < 1) { //if the number is too small we round down to 0
        physComp.velocity.x = 0;
    }

    eMgr->setComponentData<TransformComponent>(eID, transComp);
    eMgr->setComponentData<PhysicsComponent>(eID, physComp);
}
