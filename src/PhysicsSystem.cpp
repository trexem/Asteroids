#include "PhysicsSystem.h"
#include <ostream>

PhysicsSystem::PhysicsSystem(EntityManager* eManager) : eManager(eManager) {
    std::cout << " PhysicsSystem Subscribing to: ";
    // Subscribe to ApplyForceMessage
    MessageManager::getInstance().subscribe<ApplyForceMessage>(
        [this](std::shared_ptr<ApplyForceMessage> msg) { handleApplyForceMessage(msg); }
    );

}

void PhysicsSystem::handleApplyForceMessage(std::shared_ptr<ApplyForceMessage> msg) {
    std::cout << "Checking entity ID: " << msg->eID << std::endl;
    if (msg->eID >= eManager->maxEntities || !eManager->entityExists(msg->eID)) {
        std::cerr << "Error: Entity ID " << msg->eID << " does not exist!" << std::endl;
        return;
    }
    std::cout << "Entity " << msg->eID << " mask: " << eManager->entityComponentMasks[msg->eID] << std::endl;
    if (eManager->hasComponent<PhysicsComponent>(msg->eID)) {
        PhysicsComponent physics = eManager->getComponentData<PhysicsComponent>(msg->eID);
        PlayerComponent playerComp = eManager->getComponentData<PlayerComponent>(msg->eID);
        StatsComponent statsComp = eManager->getComponentData<StatsComponent>(msg->eID);
        if (playerComp.shipType) {
            physics.velocity.y += msg->force.y * statsComp.speed;
            physics.rotVelocity += msg->force.x * statsComp.rotationSpeed;
        } else {
            physics.velocity += msg->force * statsComp.speed;
        }
        if (physics.velocity.y > statsComp.maxSpeed) { 
            physics.velocity.y = statsComp.maxSpeed; 
        } else if (physics.velocity.y < statsComp.minSpeed) {
            physics.velocity.y = statsComp.minSpeed;
        }
        if (physics.velocity.x > statsComp.maxSpeed) { 
            physics.velocity.x = statsComp.maxSpeed; 
        } else if (physics.velocity.x < statsComp.minSpeed) {
            physics.velocity.x = statsComp.minSpeed; 
        }
        if (physics.rotVelocity > statsComp.maxRotationSpeed) { 
            physics.rotVelocity = statsComp.maxRotationSpeed; 
        } else if (physics.rotVelocity < -statsComp.maxRotationSpeed) { 
            physics.rotVelocity = -statsComp.maxRotationSpeed;
        }
        eManager->setComponentData<PhysicsComponent>(msg->eID, physics);
    }
    
}
