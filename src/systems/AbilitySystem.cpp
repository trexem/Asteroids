#include "AbilitySystem.h"

AbilitySystem::AbilitySystem(EntityManager* eManager) : eManager(eManager) {
    std::cout << " AbilitySystem Subscribing to: ";
    // Subscribe to KeyboardMessages
    MessageManager::getInstance().subscribe<AbilityMessage>(
        [this](std::shared_ptr<AbilityMessage> msg) { handleAbilityMessage(msg); }
    );
}

void AbilitySystem::handleAbilityMessage(std::shared_ptr<AbilityMessage> msg) {
    switch (msg->ability) {
    case ShipAbilities::LaserGun:
        spawnLaserGun(msg->eID);
        break;
    
    default:
        break;
    }
}

void AbilitySystem::spawnLaserGun(uint32_t eID) {
    ShipAbilities ability = ShipAbilities::LaserGun;
    PlayerComponent* player = eManager->getComponentDataPtr<PlayerComponent>(eID);
    StatsComponent* stats = eManager->getComponentDataPtr<StatsComponent>(eID);
    TransformComponent* transform = eManager->getComponentDataPtr<TransformComponent>(eID);
    RenderComponent* render = eManager->getComponentDataPtr<RenderComponent>(eID);
    // Entity Type
    TypeComponent type = EntityType::Shot;
    // Entity Physics
    PhysicsComponent laserPhysics;
    laserPhysics.velocity = abilitiesProjectileSpeed[ability][player->abilityLevels[ability]] * stats->projectileSpeed;
    // Entity Damage
    DamageComponent damage;
    damage.damage = abilitiesDamage[ability][player->abilityLevels[ability]] * stats->baseDamage;
    // Entity Render
    RenderComponent laserTexture;
    laserTexture.texture = &g_shot_texture;
    // Collision
    CollisionComponent laserCollider;
    laserCollider.height = laserTexture.texture->getHeight();
	laserCollider.width = laserTexture.texture->getWidth();
    
    // Transform
    double radians = transform->rotDegrees * PI / 180;
    int laserWidth = laserTexture.texture->getWidth();
    int laserWidthHalfed = laserTexture.texture->getWidth() / 2;
    int shipWidth = render->texture->getWidth() / 2;
    int shipHeight = render->texture->getHeight() / 2;
    float sinShipHeight = sin(radians) * shipHeight;
    float cosShipHeight = cos(radians) * shipHeight;

    int shots = abilitiesProjectileCount[ability][player->abilityLevels[ability]] + stats->projectileCount;
    int shotsWidthHalfed = ((shots - 1) * (3 + laserWidth)) / 2;
    std::cout << "Ship Pos: " << transform->position.x << " " << transform->position.y << " Ship rotation: "  
        << transform->rotDegrees << " Ship width/height halfed: " << shipWidth << " " << shipHeight 
        << " Shots: " << shots << " ShotsWidthHalfed: " << shotsWidthHalfed << std::endl;
    for (int i = 0; i < shots; i++) {
        // To shoot always from the center top of the ship.
        FPair position;
        position.x = transform->position.x + sinShipHeight + shipWidth - laserWidthHalfed
            - shotsWidthHalfed + i * ((3 + laserWidth));
        position.y = transform->position.y + shipHeight - cosShipHeight;
        std::cout << "i: " << i << " Position: " << position.x << " " << position.y << std::endl;
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
