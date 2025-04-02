#include "DamageSystem.h"

#include <iostream>

DamageSystem::DamageSystem(EntityManager* eManager) : eManager(eManager) {
    MessageManager::getInstance().subscribe<CollisionMessage>(
        [this](std::shared_ptr<CollisionMessage> msg) { handleCollisionMessage(msg); }
    );
}

void DamageSystem::handleCollisionMessage(std::shared_ptr<CollisionMessage> msg) {
    uint32_t entityA = msg->entityID.at(0);
    uint32_t entityB = msg->entityID.at(1);
    // std::cout << "Collision between: " << entityA << " & " << entityB << std::endl;
    if (!eManager->entityExists(entityA) || !eManager->entityExists(entityB)) {
        return;
    }

    TypeComponent typeA = eManager->getComponentData<TypeComponent>(entityA);
    TypeComponent typeB = eManager->getComponentData<TypeComponent>(entityB);
    // std::cout << "Collision types: " << static_cast<int>(typeA.type) << " & " << static_cast<int>(typeB.type) << std::endl;
    std::unordered_set<EntityType> entitiesSet = {typeA.type, typeB.type};
    if (entitiesSet == SHOT_ASTEROID) {
        uint32_t shot = (typeA.type == EntityType::Shot) ? entityA : entityB;
        uint32_t asteroid = (shot == entityA) ? entityB : entityA;
        handleAsteroidShotCollision(shot, asteroid);
    } else if (entitiesSet == PLAYER_ASTEROID) {
        uint32_t player = (typeA.type == EntityType::Player) ? entityA : entityB;
        uint32_t asteroid = (player == entityA) ? entityB : entityA;
        handleAsteroidShipCollision(player, asteroid);
    }
}

void DamageSystem::handleAsteroidShotCollision(uint32_t shot, uint32_t asteroid) {
    DamageComponent shotDamage = eManager->getComponentData<DamageComponent>(shot);
    HealthComponent asteroidHealth = eManager->getComponentData<HealthComponent>(asteroid);
    float lifeRemaining = asteroidHealth.health - shotDamage.damage;
    // std::cout << "LifeRemaining: " << lifeRemaining << std::endl;
    if (lifeRemaining > 0) {
        // std::cout << "Destroying " << shot << std::endl;
        eManager->destroyEntity(shot);
        asteroidHealth.health = lifeRemaining;
        eManager->setComponentData<HealthComponent>(asteroid, asteroidHealth);
        MessageManager::getInstance().sendMessage(std::make_shared<AnimationMessage>(asteroid, Animation::Damage));
    } else if (lifeRemaining < 0) {
        // std::cout << "Destroying " << asteroid << std::endl;
        eManager->destroyEntity(asteroid);
        shotDamage.damage = -lifeRemaining;
        eManager->setComponentData<DamageComponent>(shot, shotDamage);
    } else {
        // std::cout << "Destroying " << asteroid << " & " << shot << std::endl;
        eManager->destroyEntity(shot);
        eManager->destroyEntity(asteroid);
    }
    
}

void DamageSystem::handleAsteroidShipCollision(uint32_t ship, uint32_t asteroid) {
    // std::cout << "Handling Asteroid&Ship Collision" << std::endl;
    MessageManager::getInstance().sendMessage(std::make_shared<AnimationMessage>(ship, Animation::Damage));
}
