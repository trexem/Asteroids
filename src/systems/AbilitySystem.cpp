#include "AbilitySystem.h"
#include "GameStateManager.h"
#include "SpatialGridBuilder.h"
#include "EntityHandle.h"
#include "MessageManager.h"
#include "CollisionMessage.h"
#include "Log.h"

AbilitySystem::AbilitySystem(EntityManager& eManager, SDL_Renderer* renderer) : eMgr(eManager) {
    // Subscribe to AbilityMessages
    MessageManager::instance().subscribe<AbilityMessage>(
        [this](std::shared_ptr<AbilityMessage> msg) { handleAbilityMessage(msg); }
    );
    // Subscribe to ExplodeMessages
    MessageManager::instance().subscribe<ExplodeMessage>(
        [this](std::shared_ptr<ExplodeMessage> msg) { handleExplodeMessage(msg); }
    );
}

AbilitySystem::~AbilitySystem() {
}

void AbilitySystem::handleAbilityMessage(std::shared_ptr<AbilityMessage> msg) {
    switch (msg->ability) {
    case WeaponAbilities::LaserGun:
        spawnLaserGun(msg->eID);
        break;
    case WeaponAbilities::Rocket:
        spawnRocket(msg->eID);
        break;
    case WeaponAbilities::GravitySaws:
        spawnGravitySaws(msg->eID);
        break;
    case WeaponAbilities::Laser:
        spawnLaser(msg->eID);
        break;
    case WeaponAbilities::Explosives:
        spawnExplosives(msg->eID);
        break;
    case WeaponAbilities::TeslaGun:
        shootTeslaGun(msg->eID);
    default:
        break;
    }
}

void AbilitySystem::spawnLaserGun(uint32_t eID) {
    spawnProjectile(eID, WeaponAbilities::LaserGun);
}

void AbilitySystem::spawnRocket(uint32_t eID) {
    spawnProjectile(eID, WeaponAbilities::Rocket);
}

void AbilitySystem::handleExplodeMessage(std::shared_ptr<ExplodeMessage> msg) {
    TransformComponent* transComp = eMgr.getComponentDataPtr<TransformComponent>(msg->id);
    explosions.push_back({transComp->position, msg->source});
    eMgr.destroyEntityLater(msg->id);
}

void AbilitySystem::update(EntityManager& eMgr, const double& dT) {
    if (GameStateManager::instance().getState() == GameState::Playing) {
        for (int i = 0; i < explosions.size(); i++) {
            createExplosion(explosions[i]);
        }
        explosions.clear();
    }
}

void AbilitySystem::createExplosion(const ExplosionConfig& exp) {
    uint32_t playerID = eMgr.getEntitiesWithComponent(ComponentType::Player).at(0);
    PlayerComponent* playerComp = eMgr.getComponentDataPtr<PlayerComponent>(playerID);
    StatsComponent* statsComp = eMgr.getComponentDataPtr<StatsComponent>(playerID);
    size_t ability = static_cast<size_t>(exp.source);
    uint32_t eID = eMgr.createEntity();
    TransformComponent transComp;
    RenderComponent rendComp;
    CollisionComponent colComp;
    TypeComponent typeComp = EntityType::Explosion;
    LifeTimeComponent lifeComp;
    DamageComponent damageComp;
    PhysicsComponent physComp;
    rendComp.texture = TextureManager::instance().get("explosion");
    rendComp.size = abilitiesSize[ability][playerComp->weaponLevels[ability]] * statsComp->extraSize + 1;
    float w = rendComp.texture->getWidth() * rendComp.size;
    float h = rendComp.texture->getHeight() * rendComp.size;
    transComp.position = {exp.pos.x - w / 2, exp.pos.y - h / 2};
    colComp.shape = Shape::Circle;
    colComp.radius = std::max(w / 2.0f, h / 2.0f);
    lifeComp.lifeTime = EXPLOSION_LIFETIME;
    damageComp.damage = abilitiesDamage[ability][playerComp->weaponLevels[ability]] * statsComp->baseDamage;
    eMgr.addComponent(eID, ComponentType::Transform);
    eMgr.addComponent(eID, ComponentType::Render);
    eMgr.addComponent(eID, ComponentType::Collision);
    eMgr.addComponent(eID, ComponentType::Type);
    eMgr.addComponent(eID, ComponentType::LifeTime);
    eMgr.addComponent(eID, ComponentType::Damage);
    eMgr.addComponent(eID, ComponentType::Physics);
    eMgr.setComponentData(eID, transComp);
    eMgr.setComponentData(eID, rendComp);
    eMgr.setComponentData(eID, colComp);
    eMgr.setComponentData(eID, typeComp);
    eMgr.setComponentData(eID, lifeComp);
    eMgr.setComponentData(eID, damageComp);
    eMgr.setComponentData(eID, physComp);
}

void AbilitySystem::spawnGravitySaws(uint32_t eID) {
    spawnProjectile(eID, WeaponAbilities::GravitySaws);
}

void AbilitySystem::spawnLaser(uint32_t eID) {
    spawnProjectile(eID, WeaponAbilities::Laser);
}

void AbilitySystem::spawnExplosives(uint32_t eID) {
    spawnProjectile(eID, WeaponAbilities::Explosives);
}

auto AbilitySystem::getPlayerComponents(uint32_t eID) {
    return std::make_tuple(
        eMgr.getComponentDataPtr<PlayerComponent>(eID),
        eMgr.getComponentDataPtr<StatsComponent>(eID),
        eMgr.getComponentDataPtr<TransformComponent>(eID),
        eMgr.getComponentDataPtr<RenderComponent>(eID)
    );
}

void AbilitySystem::spawnProjectile(uint32_t eID, WeaponAbilities ability) {
    const auto& config = abilityConfigs[ability];
    const size_t abilityIndex = static_cast<size_t>(ability);
    
    auto [player, stats, transform, render] = getPlayerComponents(eID);
    unsigned int level = player->weaponLevels[abilityIndex];
    const FPair playerPos = transform->position;
    const FPair playerWH = {render->exactSize.x, render->exactSize.y};
    
    float speed = abilitiesProjectileSpeed[abilityIndex][level]  * stats->projectileSpeed;
    float damage = abilitiesDamage[abilityIndex][level] * stats->baseDamage;
    int count = player->isBursting[abilityIndex] ? 1 :
        abilitiesProjectileCount[abilityIndex][level] + stats->projectileCount;
    float size = abilitiesSize[abilityIndex][level] * stats->extraSize;
    
    RenderComponent textureComp = {config.texture, size};
    DamageComponent damageComp = {ability == WeaponAbilities::Explosives ? 0 : damage};
    CollisionComponent colComp;
    colComp.shape = config.shape;
    FPair wh = {config.texture->getWidth() * size, config.texture->getHeight() * size};
    if (colComp.shape == Shape::Rectangle) {
        colComp.width = wh.x;
        colComp.height = wh.y;
    } else if (colComp.shape == Shape::Circle) {
        colComp.radius = std::max(wh.x / 2.0f, wh.y / 2.0f);
    }
    LifeTimeComponent lifeComp;
    if (config.hasLifetime) {
        lifeComp.lifeTime = abilitiesLifeTime[abilityIndex][level];
    }
    PhysicsComponent physComp;
    if (config.accelerates) {
        physComp.acceleration = speed;
    } else {
        physComp.velocity = speed;
    }
    TransformComponent transComp;
    float angleStep = TAU / count;
    float startAngle = transform->rotDegrees * DEG2RAD;
    for (int i = 0; i < count; i++) {
        auto entity = createProjectileEntity();

        float currentAngle = startAngle + (i * angleStep);
        entity.add<TransformComponent>();
        FPair pos = positionProjectile(i, count, playerPos, transform->rotDegrees, playerWH, ability);
        transComp.position = pos;
        transComp.rotDegrees = transform->rotDegrees;
        
        if (ability == WeaponAbilities::GravitySaws) {
            entity.add<OrbitComponent>();
            physComp.velocity = 0;
            OrbitComponent orbit;
            orbit.parentId = eID;
            orbit.rotationSpeed = speed;
            orbit.radius = (std::max(playerWH.x, playerWH.y) + 100) * size;
            orbit.angle = currentAngle - PI / 2;
            entity.set<OrbitComponent>(orbit);
        } else if (ability == WeaponAbilities::Laser) {
            textureComp.isStretched = true;
            textureComp.exactSize = {size * textureComp.texture->getWidth(), size * 250.0f};
            textureComp.texture->rotationPoint = RotationPoint::BottomCenter;
            colComp.rotationPoint = RotationPoint::BottomCenter;
            colComp.width = textureComp.exactSize.x;
            colComp.height = textureComp.exactSize.y;
            physComp.velocity = 0;
            entity.add<FollowComponent>();
            FollowComponent follow;
            follow.parentId = eID;
            follow.offsetAngle = 0;
            follow.offsetPosition = { 
                render->exactSize.x / 2.0f - colComp.width / 2.0f,
                render->exactSize.y / 2.0f
            };
            entity.set<FollowComponent>(follow);
        } else if (ability == WeaponAbilities::Rocket) {
            transComp.rotDegrees = (currentAngle + HALF_PI) * RAD2DEG;
        }
        
        entity.add<TypeComponent>();
        entity.set<TypeComponent>(config.type);

        entity.add<RenderComponent>();
        entity.set<RenderComponent>(textureComp);

        entity.add<DamageComponent>();
        entity.set<DamageComponent>(damageComp);

        entity.add<PhysicsComponent>();
        entity.set<PhysicsComponent>(physComp);

        if (config.hasLifetime) {
            entity.add<LifeTimeComponent>();
            entity.set<LifeTimeComponent>(lifeComp);
        }
        
        
        entity.set<TransformComponent>(transComp);
        entity.add<CollisionComponent>();
        entity.set<CollisionComponent>(colComp);
    }
}

EntityHandle AbilitySystem::createProjectileEntity() {
    uint32_t id = eMgr.createEntity();
    return {id, eMgr};
}

FPair AbilitySystem::positionProjectile(int index, int total, const FPair& posSource, const float& angleSource,
                    const FPair& whSource, WeaponAbilities ability
) {
    switch (ability) {
    case WeaponAbilities::LaserGun:
        return positionLinearSpread(index, total, posSource, angleSource, whSource);
    case WeaponAbilities::Rocket:
        return positionRadialSpread(index, total, posSource, angleSource, whSource);
    case WeaponAbilities::GravitySaws:
        return positionRadialSpread(index, total, posSource, angleSource, whSource, -HALF_PI);
    case WeaponAbilities::Explosives:
        return positionLinearSpread(index, total, posSource, angleSource, whSource, PI);
    case WeaponAbilities::Laser:
        return positionLinearSpread(index, total, posSource, angleSource, whSource);
    default:
        return posSource;
    }
} 

FPair AbilitySystem::positionLinearSpread(int index, int total, const FPair& posSource, const float& angleSource,
    const FPair& whSource, float angleOffset) {
    double radians = angleSource * DEG2RAD + angleOffset;
    double sinRadians = sin(radians);
    double cosRadians = cos(radians);
    int laserWidth = g_shot_texture.getWidth();
    int laserWidthHalfed = g_shot_texture.getWidth() / 2;
    int shipWidth = whSource.x / 2;
    int shipHeight = whSource.y / 2;
    float sinShipHeight = sinRadians * shipHeight;
    float cosShipHeight = cosRadians * shipHeight;
    int shotsWidthHalved = ((total - 1) * (3 + laserWidth)) / 2;

    FPair position;
    float xOffset = - laserWidthHalfed - shotsWidthHalved + index * ((5 + laserWidth));
    xOffset *= cosRadians;
    float yOffset = - laserWidthHalfed - shotsWidthHalved + index * ((5 + laserWidth));
    yOffset *= sinRadians;
    yOffset += abs(2 * laserWidth * sinRadians) * (-1);
    position.x = posSource.x + shipWidth  + sinShipHeight + xOffset;
    position.y = posSource.y + shipHeight - cosShipHeight + yOffset;
    return position;
}

FPair AbilitySystem::positionRadialSpread(int index, int total, const FPair& posSource, const float& angleSource,
    const FPair& whSource, float angleOffset, float extraRadius) {
    float angleStep = TAU / total;
    float startAngle = angleSource * DEG2RAD + angleOffset;
    float radius = std::max(whSource.x, whSource.y) + extraRadius;
    float angle = startAngle + (index * angleStep);
    float xOffset = radius * cos(angle);
    float yOffset = radius * sin(angle);
    return {posSource.x + whSource.x / 2.0f + xOffset, posSource.y + whSource.y / 2.0f + yOffset};
}

void AbilitySystem::shootTeslaGun(uint32_t eID) {
    const size_t abilityIndex = static_cast<size_t>(WeaponAbilities::TeslaGun);
    auto [player, stats, transform, render] = getPlayerComponents(eID);
    unsigned int level = player->weaponLevels[abilityIndex];
    int bounces = abilitiesProjectileCount[abilityIndex][level] + stats->projectileCount;

    float range = abilitiesSize[abilityIndex][level] * stats->extraSize;
    float rangeSq = range * range;
    auto playerProx = eMgr.getComponentData<ProximityTrackerComponent>(eID);
    DEBUG_LOG("Trying to shoot tesla");
    if (playerProx.closest.has_value()) {
        if (playerProx.distance <= rangeSq) {
            uint32_t current = eID, closest = *playerProx.closest;
            FPair seekerPos, targetPos;
            float seekerRadius, targetRadius, iterDist;
            std::vector<bool> visited(eMgr.maxEntities, false);
            DEBUG_LOG("Trying to shoot tesla. Now in range");
            for (int bounce = 0; bounce < bounces; ++bounce) {
                if (closest == 0) {
                    visited[current] = true;
                    seekerPos = eMgr.getComponentData<TransformComponent>(current).position;
                    seekerRadius = eMgr.getComponentDataPtr<CollisionComponent>(current)->getBoundingRadius();

                    iterDist = std::numeric_limits<float>::max();
                    const auto& candidates = SpatialGrid::instance().getNearbyEntities(seekerPos, range, visited);
                    for (auto target : candidates) {
                        if (target == current) continue;
                        if (!eMgr.hasComponent<TypeComponent>(target)) continue;
                        auto targetType = eMgr.getComponentData<TypeComponent>(target).type;
                        if (targetType != EntityType::Asteroid) continue;

                        float targetRadius = eMgr.getComponentDataPtr<CollisionComponent>(target)->getBoundingRadius();
                        targetPos = eMgr.getComponentData<TransformComponent>(target).position;

                        float distSq = getSquaredDistanceBetweenCircles(seekerPos, seekerRadius, targetPos, targetRadius);

                        if (distSq <= rangeSq && distSq < iterDist) {
                            closest = target;
                            iterDist = distSq;
                        }
                    }
                    if (closest == 0) break;
                }

                DEBUG_LOG("Tesla gun shot, lightning component creation, bounce %d", bounce);
                if (!eMgr.entityExists(current)) break;
                if (!eMgr.entityExists(closest)) break;
                uint32_t lightingId = eMgr.createEntity();
                EntityHandle handle = {lightingId, eMgr};
                handle.add<TypeComponent>();
                handle.add<LightningComponent>();
                handle.add<LifeTimeComponent>();
                handle.add<DamageComponent>();
                LightningComponent lightningComp;
                lightningComp.sourcePos = eMgr.getComponentData<TransformComponent>(current).position
                    + eMgr.getComponentData<CollisionComponent>(current).getBoundingRadius();
                lightningComp.targetPos = eMgr.getComponentData<TransformComponent>(closest).position
                    + eMgr.getComponentData<CollisionComponent>(closest).getBoundingRadius();
                lightningComp.path = generateLightningBoltPath(lightningComp.sourcePos, lightningComp.targetPos);
                handle.set(lightningComp);
                LifeTimeComponent lifeComp;
                lifeComp.lifeTime = 0.2;
                handle.set(lifeComp);
                TypeComponent type = EntityType::Ligthning;
                handle.set(type);
                DamageComponent damage;
                damage.damage = abilitiesDamage[abilityIndex][level];
                handle.set(damage);

                std::vector<uint32_t> ids;
                ids.push_back(lightingId);
                ids.push_back(closest);
                MessageManager::instance().sendMessage(std::make_shared<CollisionMessage>(ids));

                current = closest;
                closest = 0;
            }
        }
    }
}

std::vector<FPair> AbilitySystem::generateLightningBoltPath(FPair from, FPair to, int segments, float maxOffset) {
    std::vector<FPair> path;
    path.push_back(from);

    for (int i = 1; i < segments; ++i) {
        float t = (float)i / (float)segments;
        FPair pos = from.lerp(to, t);

        float offsetAmount = maxOffset * (1.0f - std::abs(0.5f - t) * 2.0f); // strongest offset in middle

        float angle = randFloat(0, PI);
        pos.x += std::cos(angle) * offsetAmount;
        pos.y += std::sin(angle) * offsetAmount;

        path.push_back(pos);
    }

    path.push_back(to);
    return path;
}
