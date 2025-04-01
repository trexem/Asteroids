#include <cmath>
#include <iostream>

#include "MovementSystem.h"
#include "utils.hpp"

void MovementSystem::update(EntityManager* eMgr, double dT) {
    for (uint32_t eID : eMgr->getEntitiesWithComponent(ComponentType::Physics)) {
        //std::cout << "Updating movement of component: " << eID << std::endl;
        if (eMgr->hasComponent<PlayerComponent>(eID)) {
            updatePlayer(eMgr, dT, eID);
        } else {
            PhysicsComponent physComp = eMgr->getComponentData<PhysicsComponent>(eID);
            TransformComponent transComp = eMgr->getComponentData<TransformComponent>(eID);
            double radians = transComp.rotDegrees * PI / 180;
            transComp.position.x += physComp.velocity * dT * sin(radians);
            transComp.position.y -= physComp.velocity * dT * cos(radians);
            if (transComp.position.x > SCREEN_WIDTH + 500 || transComp.position.x < -500 
                || transComp.position.y > SCREEN_HEIGHT + 500 || transComp.position.y < -500) {
                    eMgr->destroyEntity(eID);
                    continue;
                }
            eMgr->setComponentData<TransformComponent>(eID, transComp);
        }
    }
}

void MovementSystem::updatePlayer(EntityManager* eMgr, double dT, uint32_t eID) {
    PhysicsComponent physComp = eMgr->getComponentData<PhysicsComponent>(eID);
    StatsComponent statsComp = eMgr->getComponentData<StatsComponent>(eID);
    TransformComponent transComp = eMgr->getComponentData<TransformComponent>(eID);
    PlayerComponent playerComp = eMgr->getComponentData<PlayerComponent>(eID);
    if (playerComp.type == ShipType::TANK) {
        double radians = transComp.rotDegrees * PI / 180;
        transComp.position.x += physComp.velocity * dT * sin(radians);
        transComp.position.y -= physComp.velocity * dT * cos(radians);
        transComp.rotDegrees += physComp.rotVelocity * dT;
        if (transComp.rotDegrees > 180) {
            transComp.rotDegrees -= 360;
        } else if (transComp.rotDegrees < -180) {
            transComp.rotDegrees += 360;
        }
    } else if (playerComp.type == ShipType::FREE_MOVE) {
        transComp.position += physComp.velocity * dT;
    }
    camera->position.x = transComp.position.x - SCREEN_CENTER.x;
    camera->position.y = transComp.position.y - SCREEN_CENTER.y;
    eMgr->setComponentData<TransformComponent>(eID, transComp);
    eMgr->setComponentData<PhysicsComponent>(eID, physComp);
}
