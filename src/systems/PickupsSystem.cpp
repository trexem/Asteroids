#include "PickupsSystem.h"
#include "GameStateManager.h"
#include "TextureManager.h"

PickupsSystem::PickupsSystem(EntityManager& eMgr, SDL_Renderer* renderer) : eMgr(eMgr) {
    MessageManager::instance().subscribe<PickupsSpawnMessage>(
        [this](std::shared_ptr<PickupsSpawnMessage> msg) { handlePickupsSpawnMessage(msg); }
    );
}

void PickupsSystem::handlePickupsSpawnMessage(std::shared_ptr<PickupsSpawnMessage> msg) {
    TransformComponent trComp;
    CollisionComponent colComp;
    PhysicsComponent physComp;
    RenderComponent renderComp;
    TypeComponent type = msg->type;
    PickupComponent pickComponent;
    if (msg->type & EntityType::Experience) {
        renderComp.texture = TextureManager::instance().get("experience");
        int lvl = GameStatsManager::instance().getUpgradeLevel(UpgradeType::Experience);
        pickComponent.value *= (1.0f + upgradesValues[static_cast<size_t>(UpgradeType::Experience)][lvl]);
    } else if (msg->type & EntityType::Gold) {
        renderComp.texture = TextureManager::instance().get("gold");
        int lvl = GameStatsManager::instance().getUpgradeLevel(UpgradeType::GoldValue);
        pickComponent.value *= (1.0f + upgradesValues[static_cast<size_t>(UpgradeType::GoldValue)][lvl]);
        std::cout << "lvl and gold value: " << lvl << ": " << pickComponent.value << std::endl;
        std::cout << "upgradesValues: " << upgradesValues[static_cast<size_t>(UpgradeType::GoldValue)][lvl] << std::endl;
    }
    colComp.height = renderComp.texture->getHeight();
    colComp.width = renderComp.texture->getWidth();
    trComp.position = {msg->position.x - renderComp.texture->getWidth() / 2, 
        msg->position.y - renderComp.texture->getHeight() / 2 };
    uint32_t id = eMgr.createEntity();
    eMgr.addComponent(id, ComponentType::Transform);
    eMgr.addComponent(id, ComponentType::Collision);
    eMgr.addComponent(id, ComponentType::Physics);
    eMgr.addComponent(id, ComponentType::Render);
    eMgr.addComponent(id, ComponentType::Pickup);
    eMgr.setComponentData<TransformComponent>(id, trComp);
    eMgr.setComponentData<CollisionComponent>(id, colComp);
    eMgr.setComponentData<PhysicsComponent>(id, physComp);
    eMgr.setComponentData<RenderComponent>(id, renderComp);
    eMgr.setComponentData<TypeComponent>(id, type);
    eMgr.setComponentData<PickupComponent>(id, pickComponent);
}

void PickupsSystem::update(EntityManager& eMgr, const double& dT) {
    if (GameStateManager::instance().getState() == GameState::Playing) {
        auto players = eMgr.getEntitiesWithComponent(ComponentType::Player);
        auto pickups = eMgr.getEntitiesWithComponent(ComponentType::Pickup);
        for (uint32_t player : players) {
        TransformComponent* playerTr = eMgr.getComponentDataPtr<TransformComponent>(player);
        StatsComponent* stats = eMgr.getComponentDataPtr<StatsComponent>(player);
        RenderComponent* playerRend = eMgr.getComponentDataPtr<RenderComponent>(player);
        const float attractionRadius = stats->collectionRadius;
        SDL_FRect playerRect = {playerTr->position.x, playerTr->position.y, 
            playerRend->texture->getWidth(), playerRend->texture->getHeight() };
        for (uint32_t xp : pickups) {
            TransformComponent xpTr = eMgr.getComponentData<TransformComponent>(xp);
            PhysicsComponent xpPhys = eMgr.getComponentData<PhysicsComponent>(xp);
            PickupComponent xpComp = eMgr.getComponentData<PickupComponent>(xp);
            if (xpComp.isPickedUp) {
                xpTr.rotDegrees = xpTr.position.angleTowards(playerTr->position) * RAD2DEG;
                eMgr.setComponentData<TransformComponent>(xp, xpTr);
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
                eMgr.setComponentData<PickupComponent>(xp, xpComp);
                eMgr.setComponentData<PhysicsComponent>(xp, xpPhys);
                eMgr.setComponentData<TransformComponent>(xp, xpTr);
            }
        }
    }
    }
}
