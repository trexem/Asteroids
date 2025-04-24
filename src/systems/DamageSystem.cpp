#include "DamageSystem.h"

#include <iostream>

DamageSystem::DamageSystem(EntityManager* eM) : eManager(eM) {
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
    if (typeA.type & typeB.type) return;
    DamageContext ctx;

    if (TypesSet::match(TypesSet::SHOT_ASTEROID, typeA.type, typeB.type) ||
        TypesSet::match(TypesSet::ROCKET_ASTEROID, typeA.type, typeB.type)) {
        ctx.source = typeA.type & (EntityType::Shot | typeA.type & EntityType::Rocket) ? entityA : entityB;
        ctx.target = ctx.source == entityA ? entityB : entityA;
        ctx.sourceType = ctx.source == entityA ? typeA.type : typeB.type;
        ctx.targetType = ctx.target == entityA ? typeA.type : typeB.type;
        ctx.damage = eManager->getComponentData<DamageComponent>(ctx.source).damage;
        ctx.cooldownDuration = 0;
        ctx.destroySource = true;
        ctx.destroyTarget = false;
        ctx.animation = Animation::Damage;
        applyDamage(ctx);
    } else if (TypesSet::match(TypesSet::PLAYER_ASTEROID, typeA.type, typeB.type)) {
        ctx.source = typeA.type & EntityType::Asteroid ? entityA : entityB;
        ctx.target = ctx.source == entityA ? entityB : entityA;
        ctx.sourceType = ctx.source == entityA ? typeA.type : typeB.type;
        ctx.targetType = ctx.target == entityA ? typeA.type : typeB.type;
        ctx.damage = eManager->getComponentData<DamageComponent>(ctx.source).damage;
        ctx.cooldownDuration = DAMAGE_COOLDOWN;
        ctx.destroySource = false;
        ctx.destroyTarget = false;
        ctx.animation = Animation::Damage;
        applyDamage(ctx);
    } else if(TypesSet::match(TypesSet::EXPLOSIVE_ASTEROID, typeA.type, typeB.type)) {
        uint32_t explosive = (typeA.type & EntityType::Explosive) ? entityA : entityB;
        uint32_t asteroid = (explosive == entityA) ? entityB : entityA;
        eManager->addComponent(explosive, ComponentType::Follow);
        FollowComponent follow;
        follow.parentId = asteroid;
        eManager->setComponentData(explosive, follow);
    } else {
        ctx.target = typeA.type & EntityType::Asteroid ? entityA : entityB;
        ctx.source = ctx.target == entityA ? entityB : entityA;
        ctx.sourceType = ctx.source == entityA ? typeA.type : typeB.type;
        ctx.targetType = ctx.target == entityA ? typeA.type : typeB.type;
        ctx.damage = eManager->getComponentData<DamageComponent>(ctx.source).damage;
        ctx.cooldownDuration = DAMAGE_COOLDOWN;
        ctx.destroySource = false;
        ctx.destroyTarget = false;
        ctx.animation = Animation::Damage;
        applyDamage(ctx);
    }
}

void DamageSystem::applyDamage(const DamageContext& ctx) {
    std::scoped_lock lock(eManager->getEntityMutex(ctx.target), 
                         eManager->getEntityMutex(ctx.source));
    if (eManager->isMarkedForDestruction(ctx.source) || eManager->isMarkedForDestruction(ctx.target)) {
        return;
    }
    auto targetIt = entitiesDamageCooldown.find(ctx.target);
    if (targetIt != entitiesDamageCooldown.end()) {
        auto sourceIt = targetIt->second.find(ctx.source);
        if (sourceIt != targetIt->second.end() && sourceIt->second > 0) {
            return; //Still on cooldown
        }
    }

    HealthComponent healthComp = eManager->getComponentData<HealthComponent>(ctx.target);
    healthComp.health -= ctx.damage;
    eManager->setComponentData(ctx.target, healthComp);
    entitiesDamageCooldown[ctx.target][ctx.source] = ctx.cooldownDuration;
    std::cout << "applying damage for target: " << ctx.targetType << " from source: " << ctx.sourceType << std::endl;
    if (healthComp.health <= 0) {
        if (ctx.targetType & EntityType::Asteroid) {
            MessageManager::getInstance().sendMessage(
                std::make_shared<DestroyAsteroidMessage>(ctx.target));
        } else if (ctx.targetType & EntityType::Player) {
            GameStateManager::getInstance().setState(GameState::GameOver);
        }
    } else {
        MessageManager::getInstance().sendMessage(
            std::make_shared<AnimationMessage>(ctx.target, ctx.animation));
    }

    if (ctx.destroySource) {
        if (ctx.sourceType & EntityType::Rocket) {
            MessageManager::getInstance().sendMessage(
                std::make_shared<ExplodeMessage>(ctx.source));
        } else {
            DamageComponent damageC = eManager->getComponentData<DamageComponent>(ctx.source);
            if (healthComp.health > 0) {
                eManager->destroyEntityLater(ctx.source);
            } else if (healthComp.health <= 0) {
                damageC.damage = -healthComp.health;
                eManager->setComponentData(ctx.source, damageC);
            }
        }
    }
}

void DamageSystem::update(double dT) {
    cleanupDeadEntities();
    // Update all cooldowns
    for (auto& [target, sources] : entitiesDamageCooldown) {
        for (auto& [source, cooldown] : sources) {
            if (cooldown > 0) {
                cooldown -= dT;
                if (cooldown <= 0) {
                    // Mark for removal (can't erase here during iteration)
                    cooldown = -1.0f;
                }
            }
        }
        
        // Remove expired cooldowns
        for (auto it = sources.begin(); it != sources.end(); ) {
            if (it->second <= 0) {
                it = sources.erase(it);
            } else {
                ++it;
            }
        }
    }

    // Clean up empty target entries
    for (auto it = entitiesDamageCooldown.begin(); it != entitiesDamageCooldown.end(); ) {
        if (it->second.empty()) {
            it = entitiesDamageCooldown.erase(it);
        } else {
            ++it;
        }
    }
}

void DamageSystem::cleanupDeadEntities() {
    for (auto targetIt = entitiesDamageCooldown.begin(); targetIt != entitiesDamageCooldown.end(); ) {
        if (eManager->isMarkedForDestruction(targetIt->first) || !eManager->entityExists(targetIt->first)) {
            targetIt = entitiesDamageCooldown.erase(targetIt);
        } else {
            // Check all sources for this target
            for (auto sourceIt = targetIt->second.begin(); sourceIt != targetIt->second.end(); ) {
                if (eManager->isMarkedForDestruction(sourceIt->first) || !eManager->entityExists(sourceIt->first)) {
                    sourceIt = targetIt->second.erase(sourceIt);
                } else {
                    ++sourceIt;
                }
            }
            ++targetIt;
        }
    }
}
