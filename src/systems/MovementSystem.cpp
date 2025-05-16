#include <cmath>
#include <iostream>

#include "MovementSystem.h"
#include "utils.hpp"
#include "GameStateManager.h"
#include "GUI.h"

void MovementSystem::update(EntityManager& eMgr, const double& dT) {
    if (GameStateManager::instance().getState() == GameState::Playing) {
        TransformComponent* playerTransform;
        for (uint32_t eID : eMgr.getEntitiesWithComponent(ComponentType::Physics)) {
            if (eMgr.hasComponent<PlayerComponent>(eID)) {
                updatePlayer(eMgr, dT, eID);
                playerTransform = eMgr.getComponentDataPtr<TransformComponent>(eID);
            } else {
                PhysicsComponent* physComp = eMgr.getComponentDataPtr<PhysicsComponent>(eID);
                TransformComponent transComp = eMgr.getComponentData<TransformComponent>(eID);
                TypeComponent* type = eMgr.getComponentDataPtr<TypeComponent>(eID);
                if (physComp->acceleration == 0 && physComp->velocity == 0 && 
                    physComp->speed.x == 0 && physComp->speed.y == 0 && physComp->rotVelocity == 0) {
                    continue;
                }
                if (physComp->isSpeedVector) {
                    transComp.position.x += physComp->speed.x * dT;
                    transComp.position.y += physComp->speed.y * dT;
                } else {
                    double radians = transComp.rotDegrees * DEG2RAD;
                    transComp.position.x += physComp->velocity * dT * sin(radians);
                    transComp.position.y -= physComp->velocity * dT * cos(radians);
                }
                if (TypesSet::shouldDestroyIfFar(type->type)) {
                    if (transComp.position.x > (playerTransform->position.x + GUI::screenWidth * 3)
                        || transComp.position.x < playerTransform->position.x - GUI::screenWidth * 2
                        || transComp.position.y > playerTransform->position.y + GUI::screenHeight * 3 
                        || transComp.position.y < playerTransform->position.y - GUI::screenHeight * 2) {
                        if (type->type & EntityType::Asteroid) {
                            MessageManager::instance().sendMessage(std::make_shared<DestroyAsteroidMessage>(eID, false));
                            continue;
                        }
                        eMgr.destroyEntityLater(eID);
                        continue;
                    }
                }
                eMgr.setComponentData<TransformComponent>(eID, transComp);
            }
        }
    }
}

void MovementSystem::updatePlayer(EntityManager& eMgr, const double& dT, uint32_t eID) {
    const PhysicsComponent* physComp = eMgr.getComponentDataPtr<PhysicsComponent>(eID);
    StatsComponent statsComp = eMgr.getComponentData<StatsComponent>(eID);
    TransformComponent transComp = eMgr.getComponentData<TransformComponent>(eID);
    PlayerComponent playerComp = eMgr.getComponentData<PlayerComponent>(eID);
    if (playerComp.type == ShipType::TANK) {
        double radians = transComp.rotDegrees * DEG2RAD;
        transComp.position.x += physComp->velocity * dT * sin(radians);
        transComp.position.y -= physComp->velocity * dT * cos(radians);
        transComp.rotDegrees += physComp->rotVelocity * dT;
        if (transComp.rotDegrees > 180) {
            transComp.rotDegrees -= 360;
        } else if (transComp.rotDegrees < -180) {
            transComp.rotDegrees += 360;
        }
    } else if (playerComp.type == ShipType::FREE_MOVE) {
        transComp.position.x += physComp->speed.x * dT;
        transComp.position.y += physComp->speed.y * dT;
    }
    camera->position.x = transComp.position.x - GUI::screenCenter.x + g_ship_surface.getWidth() /2 ;
    camera->position.y = transComp.position.y - GUI::screenCenter.y + g_ship_surface.getHeight() /2 ;
    eMgr.setComponentData<TransformComponent>(eID, transComp);
}
