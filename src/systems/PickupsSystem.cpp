#include "PickupsSystem.h"

PickupsSystem::PickupsSystem(EntityManager* eMgr, SDL_Renderer* renderer) : eManager(eMgr) {
    MessageManager::instance().subscribe<PickupsSpawnMessage>(
        [this](std::shared_ptr<PickupsSpawnMessage> msg) { handlePickupsSpawnMessage(msg); }
    );
    experienceTexture.m_renderer = renderer;
    experienceTexture.loadFromFile("data/img/experience.bmp");
    goldTexture.m_renderer = renderer;
    goldTexture.loadFromFile("data/img/gold.bmp");
}

void PickupsSystem::handlePickupsSpawnMessage(std::shared_ptr<PickupsSpawnMessage> msg) {
    TransformComponent trComp;
    CollisionComponent colComp;
    PhysicsComponent physComp;
    RenderComponent renderComp;
    TypeComponent type = msg->type;
    PickupComponent pickComponent;
    if (msg->type & EntityType::Experience) {
        renderComp.texture = &experienceTexture;
        int lvl = GameStatsManager::instance().getUpgradeLevel(UpgradeType::EXPERIENCE);
        pickComponent.value *= (1.0f + upgradesValues[static_cast<size_t>(UpgradeType::EXPERIENCE)][lvl]);
    } else if (msg->type & EntityType::Gold) {
        renderComp.texture = &goldTexture;
        int lvl = GameStatsManager::instance().getUpgradeLevel(UpgradeType::GoldValue);
        pickComponent.value *= (1.0f + upgradesValues[static_cast<size_t>(UpgradeType::GoldValue)][lvl]);
    }
    colComp.height = renderComp.texture->getHeight();
    colComp.width = renderComp.texture->getWidth();
    trComp.position = {msg->position.x - renderComp.texture->getWidth() / 2, 
        msg->position.y - renderComp.texture->getHeight() / 2 };
    uint32_t id = eManager->createEntity();
    eManager->addComponent(id, ComponentType::Transform);
    eManager->addComponent(id, ComponentType::Collision);
    eManager->addComponent(id, ComponentType::Physics);
    eManager->addComponent(id, ComponentType::Render);
    eManager->addComponent(id, ComponentType::Pickup);
    eManager->setComponentData<TransformComponent>(id, trComp);
    eManager->setComponentData<CollisionComponent>(id, colComp);
    eManager->setComponentData<PhysicsComponent>(id, physComp);
    eManager->setComponentData<RenderComponent>(id, renderComp);
    eManager->setComponentData<TypeComponent>(id, type);
    eManager->setComponentData<PickupComponent>(id, pickComponent);
}

void PickupsSystem::update() {
    auto players = eManager->getEntitiesWithComponent(ComponentType::Player);
    auto pickups = eManager->getEntitiesWithComponent(ComponentType::Pickup);
    for (uint32_t player : players) {
        TransformComponent* playerTr = eManager->getComponentDataPtr<TransformComponent>(player);
        StatsComponent* stats = eManager->getComponentDataPtr<StatsComponent>(player);
        RenderComponent* playerRend = eManager->getComponentDataPtr<RenderComponent>(player);
        const float attractionRadius = stats->collectionRadius;
        SDL_FRect playerRect = {playerTr->position.x, playerTr->position.y, 
            playerRend->texture->getWidth(), playerRend->texture->getHeight() };
        for (uint32_t xp : pickups) {
            TransformComponent xpTr = eManager->getComponentData<TransformComponent>(xp);
            PhysicsComponent xpPhys = eManager->getComponentData<PhysicsComponent>(xp);
            PickupComponent xpComp = eManager->getComponentData<PickupComponent>(xp);
            if (xpComp.isPickedUp) {
                xpTr.rotDegrees = xpTr.position.angleTowards(playerTr->position) * RAD2DEG;
                eManager->setComponentData<TransformComponent>(xp, xpTr);
                continue;
            }
            SDL_FRect xpRect = {xpTr.position.x, xpTr.position.y,
                experienceTexture.getWidth(), experienceTexture.getHeight() };
            float distSq = getSquaredDistanceBetweenCenters(playerRect, xpRect);
            if (distSq <= attractionRadius * attractionRadius) {
                const float dist = sqrtf(distSq);
                xpTr.rotDegrees = xpTr.position.angleTowards(playerTr->position) * RAD2DEG;
                const float normalizedDist = 1.0f - (dist / attractionRadius);
                xpPhys.velocity = stats->maxSpeed + 35.0f;
                xpPhys.acceleration = 1000.0f;
                xpComp.isPickedUp = true;
                eManager->setComponentData<PickupComponent>(xp, xpComp);
                eManager->setComponentData<PhysicsComponent>(xp, xpPhys);
                eManager->setComponentData<TransformComponent>(xp, xpTr);
            }
        }
    }
}
