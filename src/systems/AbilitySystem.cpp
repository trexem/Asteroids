#include "AbilitySystem.h"

AbilitySystem::AbilitySystem(EntityManager* eManager, SDL_Renderer* renderer) : eManager(eManager) {
    // Subscribe to AbilityMessages
    MessageManager::getInstance().subscribe<AbilityMessage>(
        [this](std::shared_ptr<AbilityMessage> msg) { handleAbilityMessage(msg); }
    );
    // Subscribe to DestroyRocketMessages
    MessageManager::getInstance().subscribe<DestroyRocketMessage>(
        [this](std::shared_ptr<DestroyRocketMessage> msg) { handleDestroyRocketMessage(msg); }
    );
    explosionTexture.m_renderer = renderer;
    gravitySawTexture.m_renderer = renderer;
    explosiveTexture.m_renderer = renderer;
    laserTexture.m_renderer = renderer;
    explosionTexture.loadFromFile("data/img/explosion.bmp");
    gravitySawTexture.loadFromFile("data/img/gravitySaw.bmp");
    explosiveTexture.loadFromFile("data/img/explosive.bmp");
    laserTexture.loadFromFile("data/img/laserBeam.bmp");
}

AbilitySystem::~AbilitySystem() {
    explosionTexture.free();
}

void AbilitySystem::handleAbilityMessage(std::shared_ptr<AbilityMessage> msg) {
    switch (msg->ability) {
    case ShipAbilities::LaserGun:
        spawnLaserGun(msg->eID);
        break;
    case ShipAbilities::Rocket:
        spawnRocket(msg->eID);
        break;
    case ShipAbilities::GravitySaws:
        spawnGravitySaws(msg->eID);
    case ShipAbilities::Laser:
        spawnLaser(msg->eID);
    default:
        break;
    }
}

void AbilitySystem::spawnLaserGun(uint32_t eID) {
    spawnProjectile(eID, ShipAbilities::LaserGun);
}

void AbilitySystem::spawnRocket(uint32_t eID) {
    spawnProjectile(eID, ShipAbilities::Rocket);
}

void AbilitySystem::handleDestroyRocketMessage(std::shared_ptr<DestroyRocketMessage> msg) {
    TransformComponent* transComp = eManager->getComponentDataPtr<TransformComponent>(msg->id);
    std::cout << "Handling DestroyRocketMessage for position: " << transComp->position << std::endl;
    rocketsDestroyed.push_back(transComp->position);
    eManager->destroyEntityLater(msg->id);
}

void AbilitySystem::update() {
    for (const FPair& pos : rocketsDestroyed) {
        createExplosion(pos);
    }
    rocketsDestroyed.clear();
}

void AbilitySystem::createExplosion(const FPair& pos) {
    uint32_t playerID = eManager->getEntitiesWithComponent(ComponentType::Player).at(0);
    PlayerComponent* playerComp = eManager->getComponentDataPtr<PlayerComponent>(playerID);
    StatsComponent* statsComp = eManager->getComponentDataPtr<StatsComponent>(playerID);
    size_t ability = static_cast<size_t>(ShipAbilities::Rocket);
    std::cout << "Creating explosion at: " << pos << std::endl;
    uint32_t eID = eManager->createEntity();
    TransformComponent transComp;
    RenderComponent rendComp;
    CollisionComponent colComp;
    TypeComponent typeComp = EntityType::Explosion;
    LifeTimeComponent lifeComp;
    DamageComponent damageComp;
    PhysicsComponent physComp;
    rendComp.texture = &explosionTexture;
    rendComp.size = abilitiesSize[ability][playerComp->abilityLevels[ability]] + statsComp->extraSize + 1;
    float w = explosionTexture.getWidth() * rendComp.size;
    float h = explosionTexture.getHeight() * rendComp.size;
    transComp.position = {pos.x - w / 2, pos.y - h / 2};
    colComp.shape = Shape::Circle;
    colComp.radius = std::max(w, h);
    lifeComp.lifeTime = abilitiesLifeTime[ability][playerComp->abilityLevels[ability]];
    damageComp.damage = abilitiesDamage[ability][playerComp->abilityLevels[ability]];
    eManager->addComponent(eID, ComponentType::Transform);
    eManager->addComponent(eID, ComponentType::Render);
    eManager->addComponent(eID, ComponentType::Collision);
    eManager->addComponent(eID, ComponentType::Type);
    eManager->addComponent(eID, ComponentType::LifeTime);
    eManager->addComponent(eID, ComponentType::Damage);
    eManager->addComponent(eID, ComponentType::Physics);
    eManager->setComponentData(eID, transComp);
    eManager->setComponentData(eID, rendComp);
    eManager->setComponentData(eID, colComp);
    eManager->setComponentData(eID, typeComp);
    eManager->setComponentData(eID, lifeComp);
    eManager->setComponentData(eID, damageComp);
    eManager->setComponentData(eID, physComp);
}

void AbilitySystem::spawnGravitySaws(uint32_t eID) {
    spawnProjectile(eID, ShipAbilities::GravitySaws);
}

void AbilitySystem::spawnLaser(uint32_t eID) {
    spawnProjectile(eID, ShipAbilities::Laser);
}

void AbilitySystem::spawnExplosive(uint32_t eID) {

}

auto AbilitySystem::getPlayerComponents(uint32_t eID) {
    return std::make_tuple(
        eManager->getComponentDataPtr<PlayerComponent>(eID),
        eManager->getComponentDataPtr<StatsComponent>(eID),
        eManager->getComponentDataPtr<TransformComponent>(eID),
        eManager->getComponentDataPtr<RenderComponent>(eID)
    );
}

void AbilitySystem::spawnProjectile(uint32_t eID, ShipAbilities ability) {
    const auto& config = abilityConfigs[ability];
    const size_t abilityIndex = static_cast<size_t>(ability);
    
    auto [player, stats, transform, render] = getPlayerComponents(eID);
    const FPair playerPos = transform->position;
    const FPair playerWH = {render->texture->getWidth(), render->texture->getHeight()};
    
    float speed = abilitiesProjectileSpeed[abilityIndex][player->abilityLevels[abilityIndex]] 
        * stats->projectileSpeed;
    float damage = abilitiesDamage[abilityIndex][player->abilityLevels[abilityIndex]] 
        * stats->baseDamage;
    int count = abilitiesProjectileCount[abilityIndex][player->abilityLevels[abilityIndex]] 
        + stats->projectileCount;
    float size = abilitiesSize[abilityIndex][player->abilityLevels[abilityIndex]] + stats->extraSize;
    
    RenderComponent textureComp = {config.texture, size};
    DamageComponent damageComp = {damage};
    CollisionComponent colComp;
    colComp.shape = config.shape;
    FPair wh = {config.texture->getWidth() * size, config.texture->getHeight() * size};
    if (colComp.shape == Shape::Rectangle) {
        colComp.width = wh.x;
        colComp.height = wh.y;
    } else if (colComp.shape == Shape::Circle) {
        colComp.radius = std::max(wh.x, wh.y);
    }
    LifeTimeComponent lifeComp;
    if (config.hasLifetime) {
        lifeComp.lifeTime = abilitiesLifeTime[abilityIndex][player->abilityLevels[abilityIndex]];
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
        
        if (ability == ShipAbilities::GravitySaws) {
            entity.add<OrbitComponent>();
            physComp.velocity = 0;
            OrbitComponent orbit;
            orbit.parentId = eID;
            orbit.rotationSpeed = speed;
            orbit.radius = std::max(playerWH.x, playerWH.y) + 100;
            orbit.angle = currentAngle - PI / 2;
            entity.set<OrbitComponent>(orbit);
        } else if (ability == ShipAbilities::Laser) {
            textureComp.isStretched = true;
            textureComp.exactSize = {size * laserTexture.getWidth(), size * 250.0f};
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
                render->texture->getWidth() / 2.0f - colComp.width / 2.0f,
                render->texture->getHeight() / 2.0f
            };
            entity.set<FollowComponent>(follow);
        } else if (ability == ShipAbilities::Rocket) {
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
    uint32_t id = eManager->createEntity();
    return {id, eManager};
}

FPair AbilitySystem::positionProjectile(int index, int total, const FPair& posSource, const float& angleSource,
                    const FPair& whSource, ShipAbilities ability
) {
    switch (ability) {
    case ShipAbilities::LaserGun:
        return positionLinearSpread(index, total, posSource, angleSource, whSource);
    case ShipAbilities::Rocket:
        return positionRadialSpread(index, total, posSource, angleSource, whSource, HALF_PI);
    case ShipAbilities::GravitySaws:
        return positionRadialSpread(index, total, posSource, angleSource, whSource, -HALF_PI);
    case ShipAbilities::Explosives:
        return positionLinearSpread(index, total, posSource, angleSource, whSource, PI);
    case ShipAbilities::Laser:
        return positionLinearSpread(index, total, posSource, angleSource, whSource);
    default:
        return posSource;
    }
} 

FPair AbilitySystem::positionLinearSpread(int index, int total, const FPair& posSource, const float& angleSource,
    const FPair& whSource, float angleOffset) {
    double radians = angleSource * DEG2RAD;
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
    float xOffset = - laserWidthHalfed - shotsWidthHalved + index * ((3 + laserWidth));
    xOffset *= cosRadians;
    float yOffset = - laserWidthHalfed - shotsWidthHalved + index * ((3 + laserWidth));
    yOffset *= sinRadians;
    yOffset += abs(2 * laserWidth * sinRadians) * (-1);
    position.x = posSource.x + shipWidth  + sinShipHeight + xOffset;
    position.y = posSource.y + shipHeight - cosShipHeight + yOffset;
    return position;
}

FPair AbilitySystem::positionRadialSpread(int index, int total, const FPair& posSource, const float& angleSource,
    const FPair& whSource, float angleOffset, float extraRadius) {
    float angleStep = TAU / total;
    float startAngle = angleSource * DEG2RAD;
    float radius = std::max(whSource.x, whSource.y) + extraRadius;
    float angle = startAngle + (index * angleStep);
    float xOffset = radius * cos(angle);
    float yOffset = radius * sin(angle);
    return {posSource.x + whSource.x / 2.0f + xOffset, posSource.y + whSource.y / 2.0f + yOffset};
}
