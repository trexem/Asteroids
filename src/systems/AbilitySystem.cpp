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
    explosionTexture.loadFromFile("data/img/explosion.bmp");
    gravitySawTexture.loadFromFile("data/img/gravitySaw.bmp");
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
    default:
        break;
    }
}

void AbilitySystem::spawnLaserGun(uint32_t eID) {
    ShipAbilities ability = ShipAbilities::LaserGun;
    const size_t abilityIndex = static_cast<size_t>(ability);
    PlayerComponent* player = eManager->getComponentDataPtr<PlayerComponent>(eID);
    StatsComponent* stats = eManager->getComponentDataPtr<StatsComponent>(eID);
    TransformComponent* transform = eManager->getComponentDataPtr<TransformComponent>(eID);
    RenderComponent* render = eManager->getComponentDataPtr<RenderComponent>(eID);
    // Entity Type
    TypeComponent type = EntityType::Shot;
    // Entity Physics
    PhysicsComponent laserPhysics;
    laserPhysics.velocity = abilitiesProjectileSpeed[abilityIndex][player->abilityLevels[abilityIndex]]
        * stats->projectileSpeed;
    // Entity Damage
    DamageComponent damage;
    damage.damage = abilitiesDamage[abilityIndex][player->abilityLevels[abilityIndex]] 
        * stats->baseDamage;
    // Entity Render
    RenderComponent laserTexture;
    laserTexture.texture = &g_shot_texture;
    // Collision
    CollisionComponent laserCollider;
    laserCollider.height = laserTexture.texture->getHeight();
	laserCollider.width = laserTexture.texture->getWidth();
    
    // Transform
    double radians = transform->rotDegrees * DEG2RAD;
    double sinRadians = sin(radians);
    double cosRadians = cos(radians);
    int laserWidth = laserTexture.texture->getWidth();
    int laserWidthHalfed = laserTexture.texture->getWidth() / 2;
    int shipWidth = render->texture->getWidth() / 2;
    int shipHeight = render->texture->getHeight() / 2;
    float sinShipHeight = sinRadians * shipHeight;
    float cosShipHeight = cosRadians * shipHeight;

    int shots = abilitiesProjectileCount[abilityIndex][player->abilityLevels[abilityIndex]] 
        + stats->projectileCount;
    int shotsWidthHalved = ((shots - 1) * (3 + laserWidth)) / 2;
    // std::cout << "Ship Pos: " << transform->position.x << " " << transform->position.y << " Ship rotation: "  
    //     << transform->rotDegrees << " Ship width/height halfed: " << shipWidth << " " << shipHeight 
    //     << " Shots: " << shots << " shotsWidthHalved: " << shotsWidthHalved << std::endl;
    for (int i = 0; i < shots; i++) {
        // To shoot always from the center top of the ship.
        FPair position;
        float xOffset = - laserWidthHalfed - shotsWidthHalved + i * ((3 + laserWidth));
        xOffset *= cosRadians;
        float yOffset = - laserWidthHalfed - shotsWidthHalved + i * ((3 + laserWidth));
        yOffset *= sinRadians;
        yOffset += abs(2 * laserWidth * sinRadians) * (-1);
        position.x = transform->position.x + shipWidth  + sinShipHeight + xOffset;
        position.y = transform->position.y + shipHeight - cosShipHeight + yOffset;
        // std::cout << "i: " << i << " Position: " << position.x << " " << position.y << std::endl;
        // Entity Transform
        TransformComponent laserTransform;
        laserTransform.position = position;
        laserTransform.rotDegrees = transform->rotDegrees;

        //Entity Creation
        uint32_t laser = eManager->createEntity();
        eManager->addComponent(laser, ComponentType::Type);
        eManager->addComponent(laser, ComponentType::Physics);
        eManager->addComponent(laser, ComponentType::Damage);
        eManager->addComponent(laser, ComponentType::Render);
        eManager->addComponent(laser, ComponentType::Collision);
        eManager->addComponent(laser, ComponentType::Transform);
        eManager->setComponentData<TypeComponent>(laser, type);
        eManager->setComponentData<PhysicsComponent>(laser, laserPhysics);
        eManager->setComponentData<DamageComponent>(laser, damage);
        eManager->setComponentData<RenderComponent>(laser, laserTexture);
        eManager->setComponentData<CollisionComponent>(laser, laserCollider);
        eManager->setComponentData<TransformComponent>(laser, laserTransform);
    }
    
}

void AbilitySystem::spawnRocket(uint32_t eID) {
    ShipAbilities ability = ShipAbilities::Rocket;
    const size_t abilityIndex = static_cast<size_t>(ability);
    PlayerComponent* player = eManager->getComponentDataPtr<PlayerComponent>(eID);
    StatsComponent* stats = eManager->getComponentDataPtr<StatsComponent>(eID);
    TransformComponent* transform = eManager->getComponentDataPtr<TransformComponent>(eID);
    RenderComponent* render = eManager->getComponentDataPtr<RenderComponent>(eID);
    // Entity Type
    TypeComponent type = EntityType::Rocket;
    // Entity Physics
    PhysicsComponent rocketPhysics;
    rocketPhysics.acceleration = abilitiesProjectileSpeed[abilityIndex][player->abilityLevels[abilityIndex]]
        * stats->projectileSpeed;
    // Entity Damage
    DamageComponent damage;
    damage.damage = abilitiesDamage[abilityIndex][player->abilityLevels[abilityIndex]] 
        * stats->baseDamage;
    // Entity Render
    RenderComponent rocketTexture;
    rocketTexture.texture = &g_rocket_texture;
    // Collision
    CollisionComponent rocketCollider;
    rocketCollider.height = rocketTexture.texture->getHeight();
	rocketCollider.width = rocketTexture.texture->getWidth();
    // Entity Transform
    TransformComponent rocketTransform;
    // LifeTime
    LifeTimeComponent lifeComp;
    lifeComp.lifeTime = abilitiesLifeTime[abilityIndex][player->abilityLevels[abilityIndex]];

    // Transform
    int shots = abilitiesProjectileCount[abilityIndex][player->abilityLevels[abilityIndex]] 
        + stats->projectileCount;
    float angleStep = 360.0f / shots;
    float startAngle = transform->rotDegrees; // Start at right side (90 degrees)
    float radius = std::max(render->texture->getHeight(), render->texture->getWidth());
    for (int i = 0; i < shots; i++) {
        float currentAngle = startAngle + (i * angleStep);
        float radians = currentAngle * DEG2RAD;

        float xOffset = radius * cos(radians);
        float yOffset = radius * sin(radians);

        rocketTransform.position.x = transform->position.x + render->texture->getWidth()/2 + xOffset;
        rocketTransform.position.y = transform->position.y + render->texture->getHeight()/2 + yOffset;
        rocketTransform.rotDegrees = currentAngle + 90.0f;

        uint32_t rocket = eManager->createEntity();
        eManager->addComponent(rocket, ComponentType::Type);
        eManager->addComponent(rocket, ComponentType::Physics);
        eManager->addComponent(rocket, ComponentType::Damage);
        eManager->addComponent(rocket, ComponentType::Render);
        eManager->addComponent(rocket, ComponentType::Collision);
        eManager->addComponent(rocket, ComponentType::Transform);
        eManager->addComponent(rocket, ComponentType::LifeTime);
        
        eManager->setComponentData<TypeComponent>(rocket, type);
        eManager->setComponentData<PhysicsComponent>(rocket, rocketPhysics);
        eManager->setComponentData<DamageComponent>(rocket, damage);
        eManager->setComponentData<RenderComponent>(rocket, rocketTexture);
        eManager->setComponentData<CollisionComponent>(rocket, rocketCollider);
        eManager->setComponentData<TransformComponent>(rocket, rocketTransform);
        eManager->setComponentData<LifeTimeComponent>(rocket, lifeComp);
    }
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
    const size_t ability = static_cast<size_t>(ShipAbilities::GravitySaws);
    PlayerComponent* player = eManager->getComponentDataPtr<PlayerComponent>(eID);
    StatsComponent* stats = eManager->getComponentDataPtr<StatsComponent>(eID);
    TransformComponent* transform = eManager->getComponentDataPtr<TransformComponent>(eID);
    RenderComponent* render = eManager->getComponentDataPtr<RenderComponent>(eID);
    //GravitySaw Type
    TypeComponent type = EntityType::GravitySaw;
    //GravitySaw Physics
    PhysicsComponent gravityPhysics;
    gravityPhysics.rotVelocity = 720;
    //GravitySaw Damage
    DamageComponent damage;
    damage.damage = abilitiesDamage[ability][player->abilityLevels[ability]] * stats->baseDamage;
    //GravitySaw Render
    RenderComponent rendComp;
    rendComp.texture = &gravitySawTexture;
    rendComp.size = abilitiesSize[ability][player->abilityLevels[ability]] + stats->extraSize;
    //GravitySaw Collider
    CollisionComponent colComp;
    colComp.shape = Shape::Circle;
    //GravitySaw Transform
    TransformComponent transComp;
    //LifeTime
    LifeTimeComponent lifeComp;
    lifeComp.lifeTime = abilitiesLifeTime[ability][player->abilityLevels[ability]];
    //Orbit Comp
    OrbitComponent orbitComp;
    orbitComp.parentId = eID;
    int shots = abilitiesProjectileCount[ability][player->abilityLevels[ability]] 
        + stats->projectileCount;
    float angleStep = TAU / shots;
    float startAngle = transform->rotDegrees; // Start at right side (90 degrees)
    float w = gravitySawTexture.getWidth() * rendComp.size;
    float h = gravitySawTexture.getHeight() * rendComp.size;
    float radius = std::max(w, h);
    colComp.radius = radius;
    orbitComp.radius = radius + 100.0f;
    orbitComp.rotationSpeed = abilitiesProjectileSpeed[ability][player->abilityLevels[ability]];
    for (int i = 0; i < shots; i++) {
        float currentAngle = startAngle + (i * angleStep);
        orbitComp.angle = currentAngle - PI / 2;

        float xOffset = radius * cos(currentAngle);
        float yOffset = radius * sin(currentAngle);

        transComp.position.x = transform->position.x + render->texture->getWidth()/2 + xOffset;
        transComp.position.y = transform->position.y + render->texture->getHeight()/2 + yOffset;

        uint32_t gravitySaw = eManager->createEntity();
        eManager->addComponent(gravitySaw, ComponentType::Type);
        eManager->addComponent(gravitySaw, ComponentType::Physics);
        eManager->addComponent(gravitySaw, ComponentType::Damage);
        eManager->addComponent(gravitySaw, ComponentType::Render);
        eManager->addComponent(gravitySaw, ComponentType::Collision);
        eManager->addComponent(gravitySaw, ComponentType::Transform);
        eManager->addComponent(gravitySaw, ComponentType::LifeTime);
        eManager->addComponent(gravitySaw, ComponentType::Orbit);
        
        eManager->setComponentData<TypeComponent>(gravitySaw, type);
        eManager->setComponentData<PhysicsComponent>(gravitySaw, gravityPhysics);
        eManager->setComponentData<DamageComponent>(gravitySaw, damage);
        eManager->setComponentData<RenderComponent>(gravitySaw, rendComp);
        eManager->setComponentData<CollisionComponent>(gravitySaw, colComp);
        eManager->setComponentData<TransformComponent>(gravitySaw, transComp);
        eManager->setComponentData<LifeTimeComponent>(gravitySaw, lifeComp);
        eManager->setComponentData<OrbitComponent>(gravitySaw, orbitComp);
    }

}
