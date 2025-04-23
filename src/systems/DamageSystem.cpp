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
    if (eManager->isMarkedForDestruction(entityA) || eManager->isMarkedForDestruction(entityB)) {
        return;
    }

    TypeComponent typeA = eManager->getComponentData<TypeComponent>(entityA);
    TypeComponent typeB = eManager->getComponentData<TypeComponent>(entityB);
    // std::cout << "Collision types: " << static_cast<int>(typeA.type) << " & " << static_cast<int>(typeB.type) << std::endl;
    std::unordered_set<EntityType> entitiesSet = {typeA.type, typeB.type};
    if (TypesSet::match(TypesSet::SHOT_ASTEROID, typeA.type, typeB.type) ||
        TypesSet::match(TypesSet::ROCKET_ASTEROID, typeA.type, typeB.type)) {
        uint32_t shot = (typeA.type & EntityType::Shot || typeA.type & EntityType::Rocket) ? entityA : entityB;
        uint32_t asteroid = (shot == entityA) ? entityB : entityA;
        handleAsteroidShotCollision(shot, asteroid);
    } else if (TypesSet::match(TypesSet::PLAYER_ASTEROID, typeA.type, typeB.type)) {
        uint32_t player = (typeA.type == EntityType::Player) ? entityA : entityB;
        uint32_t asteroid = (player == entityA) ? entityB : entityA;
        handleAsteroidShipCollision(player, asteroid);
    } else if (TypesSet::match(TypesSet::EXPLOSION_ASTEROID, typeA.type, typeB.type)) {
        uint32_t explosion = (typeA.type & EntityType::Explosion) ? entityA : entityB;
        uint32_t asteroid = (explosion == entityA) ? entityB : entityA;
        handleAsteroidExplosionCollision(explosion, asteroid);
    } else if (TypesSet::match(TypesSet::SAW_ASTEROID, typeA.type, typeB.type)) {
        uint32_t saw = (typeA.type & EntityType::GravitySaw) ? entityA : entityB;
        uint32_t asteroid = (saw == entityA) ? entityB : entityA;
        handleAsteroidSawCollision(saw, asteroid);
    } else if(TypesSet::match(TypesSet::LASER_ASTEROID, typeA.type, typeB.type)) {
        uint32_t laser = (typeA.type & EntityType::Laser) ? entityA : entityB;
        uint32_t asteroid = (laser == entityA) ? entityB : entityA;
        handleAsteroidLaserCollision(laser, asteroid);
    } else if(TypesSet::match(TypesSet::EXPLOSIVE_ASTEROID, typeA.type, typeB.type)) {
        uint32_t explosive = (typeA.type & EntityType::Explosive) ? entityA : entityB;
        uint32_t asteroid = (explosive == entityA) ? entityB : entityA;
        handleAsteroidExplosiveCollision(explosive, asteroid);
    }
}

void DamageSystem::handleAsteroidShotCollision(uint32_t shot, uint32_t asteroid) {
    DamageComponent shotDamage = eManager->getComponentData<DamageComponent>(shot);
    HealthComponent asteroidHealth = eManager->getComponentData<HealthComponent>(asteroid);
    TypeComponent* type = eManager->getComponentDataPtr<TypeComponent>(shot);
    float lifeRemaining = asteroidHealth.health - shotDamage.damage;
    if (type->type & EntityType::Rocket) {
        std::cout << "Destroying rocket after collision" << std::endl;
        MessageManager::getInstance().sendMessage(std::make_shared<ExplodeMessage>(shot));
    }
    if (lifeRemaining > 0) {
        // std::cout << "Destroying " << shot << std::endl;
        eManager->destroyEntityLater(shot);
        asteroidHealth.health = lifeRemaining;
        eManager->setComponentData<HealthComponent>(asteroid, asteroidHealth);
        MessageManager::getInstance().sendMessage(std::make_shared<AnimationMessage>(asteroid, Animation::Damage));
    } else if (lifeRemaining < 0) {
        // std::cout << "Destroying " << asteroid << std::endl;
        MessageManager::getInstance().sendMessage(std::make_shared<DestroyAsteroidMessage>(asteroid));
        shotDamage.damage = -lifeRemaining;
        eManager->setComponentData<DamageComponent>(shot, shotDamage);
    } else {
        // std::cout << "Destroying " << asteroid << " & " << shot << std::endl;
        eManager->destroyEntityLater(shot);
        MessageManager::getInstance().sendMessage(std::make_shared<DestroyAsteroidMessage>(asteroid));
    }
}

void DamageSystem::handleAsteroidExplosionCollision(uint32_t explosion, uint32_t asteroid) {
    float damage = eManager->getComponentDataPtr<DamageComponent>(explosion)->damage;
    HealthComponent asteroidHealth = eManager->getComponentData<HealthComponent>(asteroid);
    if (asteroidHealth.explosionCooldown > 0) return;
    float lifeRemaining = asteroidHealth.health - damage;
    asteroidHealth.health = lifeRemaining;
    asteroidHealth.explosionCooldown = EXPLOSION_COOLDOWN;
    eManager->setComponentData<HealthComponent>(asteroid, asteroidHealth);
    if (lifeRemaining < 0) {
        MessageManager::getInstance().sendMessage(std::make_shared<DestroyAsteroidMessage>(asteroid));
    } else {
        MessageManager::getInstance().sendMessage(std::make_shared<AnimationMessage>(asteroid, Animation::Damage));
    }
    return;
}

void DamageSystem::handleAsteroidShipCollision(uint32_t ship, uint32_t asteroid) {
    // std::cout << "Handling Asteroid&Ship Collision" << std::endl;
    MessageManager::getInstance().sendMessage(std::make_shared<AnimationMessage>(ship, Animation::Damage));
}

void DamageSystem::handleAsteroidSawCollision(uint32_t saw, uint32_t asteroid) {
    float damage = eManager->getComponentDataPtr<DamageComponent>(saw)->damage;
    HealthComponent asteroidHealth = eManager->getComponentData<HealthComponent>(asteroid);
    if (asteroidHealth.explosionCooldown > 0) return;
    float lifeRemaining = asteroidHealth.health - damage;
    asteroidHealth.health = lifeRemaining;
    asteroidHealth.explosionCooldown = EXPLOSION_COOLDOWN;
    eManager->setComponentData<HealthComponent>(asteroid, asteroidHealth);
    if (lifeRemaining < 0) {
        MessageManager::getInstance().sendMessage(std::make_shared<DestroyAsteroidMessage>(asteroid));
    } else {
        MessageManager::getInstance().sendMessage(std::make_shared<AnimationMessage>(asteroid, Animation::Damage));
    }
    return;
}

void DamageSystem::handleAsteroidLaserCollision(uint32_t laser, uint32_t asteroid) {
    float damage = eManager->getComponentDataPtr<DamageComponent>(laser)->damage;
    HealthComponent asteroidHealth = eManager->getComponentData<HealthComponent>(asteroid);
    if (asteroidHealth.laserCooldown > 0) return;
    float lifeRemaining = asteroidHealth.health - damage;
    asteroidHealth.health = lifeRemaining;
    asteroidHealth.laserCooldown = LASER_COOLDOWN;
    eManager->setComponentData<HealthComponent>(asteroid, asteroidHealth);
    if (lifeRemaining < 0) {
        MessageManager::getInstance().sendMessage(std::make_shared<DestroyAsteroidMessage>(asteroid));
    } else {
        MessageManager::getInstance().sendMessage(std::make_shared<AnimationMessage>(asteroid, Animation::Damage));
    }
    return;
}

void DamageSystem::handleAsteroidExplosiveCollision(uint32_t explosive, uint32_t asteroid) {
    eManager->addComponent(explosive, ComponentType::Follow);
    FollowComponent follow;
    follow.parentId = asteroid;
    eManager->setComponentData(explosive, follow);
}

void DamageSystem::update(double dT) {
    for (uint32_t eID : eManager->getEntitiesWithComponent(ComponentType::Health)) {
        HealthComponent healthComp = eManager->getComponentData<HealthComponent>(eID);
        if (healthComp.explosionCooldown >= 0) {
            healthComp.explosionCooldown -= dT;
        }
        if (healthComp.laserCooldown >= 0) {
            healthComp.laserCooldown -= dT;
        }
        eManager->setComponentData(eID, healthComp);
    }
}
