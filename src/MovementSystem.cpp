#include "MovementSystem.h"
#include "utils.hpp"
#include <cmath>
#include <iostream>

void MovementSystem::update(EntityManager* eMgr, double dT) {
    for (uint32_t eID : eMgr->getEntitiesWithComponent(ComponentType::Physics)) {
        std::cout << "Updating movement of component: " << eID << std::endl;
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
    std::cout << "Getting PysicsComponent for: " << eID << std::endl;
    PhysicsComponent physComp = eMgr->getComponentData<PhysicsComponent>(eID);
    std::cout << "Getting StatsComponent for: " << eID << std::endl;
    StatsComponent statsComp = eMgr->getComponentData<StatsComponent>(eID);
    std::cout << "Getting TransformComponent for: " << eID << std::endl;
    TransformComponent transComp = eMgr->getComponentData<TransformComponent>(eID);
    std::cout << "Getting PlayerComponent for: " << eID << std::endl;
    PlayerComponent playerComp = eMgr->getComponentData<PlayerComponent>(eID);
    if (playerComp.shipType) {
        double radians = transComp.rotDegrees * PI / 180;
        transComp.position.x += physComp.velocity.y * dT * sin(radians);
        transComp.position.y += physComp.velocity.y * dT * cos(radians);
        transComp.rotDegrees += physComp.rotVelocity * dT;
        if (transComp.rotDegrees > 180) {
            transComp.rotDegrees -= 360;
        } else if (transComp.rotDegrees < -180) {
            transComp.rotDegrees += 360;
        }
    } else {
        transComp.position += physComp.velocity * dT;
    }
    eMgr->setComponentData<TransformComponent>(eID, transComp);
}