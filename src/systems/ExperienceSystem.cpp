#include "ExperienceSystem.h"

ExperienceSystem::ExperienceSystem(EntityManager* eMgr, SDL_Renderer* renderer) : eManager(eMgr) {
    MessageManager::getInstance().subscribe<ExperienceSpawnMessage>(
        [this](std::shared_ptr<ExperienceSpawnMessage> msg) { handleExperienceSpawnMessage(msg); }
    );
    experienceTexture.m_renderer = renderer;
    experienceTexture.loadFromFile("data/img/experience.bmp");
}

void ExperienceSystem::handleExperienceSpawnMessage(std::shared_ptr<ExperienceSpawnMessage> msg) {
    TransformComponent trComp;
    trComp.position = {msg->position.x - experienceTexture.getWidth() / 2, 
        msg->position.y - experienceTexture.getHeight() / 2 };
    CollisionComponent colComp;
    colComp.position = trComp.position;
    colComp.height = experienceTexture.getHeight();
    colComp.width = experienceTexture.getWidth();
    PhysicsComponent physComp;
    RenderComponent renderComp;
    renderComp.texture = &experienceTexture;
    TypeComponent type = EntityType::Experience;
    ExperienceComponent xp;
    uint32_t id = eManager->createEntity();
    eManager->addComponent(id, ComponentType::Transform);
    eManager->addComponent(id, ComponentType::Collision);
    eManager->addComponent(id, ComponentType::Physics);
    eManager->addComponent(id, ComponentType::Render);
    eManager->addComponent(id, ComponentType::Experience);
    eManager->setComponentData<TransformComponent>(id, trComp);
    eManager->setComponentData<CollisionComponent>(id, colComp);
    eManager->setComponentData<PhysicsComponent>(id, physComp);
    eManager->setComponentData<RenderComponent>(id, renderComp);
    eManager->setComponentData<TypeComponent>(id, type);
    eManager->setComponentData<ExperienceComponent>(id, xp);
}

void ExperienceSystem::update() {
    auto players = eManager->getEntitiesWithComponent(ComponentType::Player);
    auto experiences = eManager->getEntitiesWithComponent(ComponentType::Experience);
    for (uint32_t player : players) {
        TransformComponent* playerTr = eManager->getComponentDataPtr<TransformComponent>(player);
        StatsComponent* stats = eManager->getComponentDataPtr<StatsComponent>(player);
        RenderComponent* playerRend = eManager->getComponentDataPtr<RenderComponent>(player);
        SDL_FRect playerRect = {playerTr->position.x, playerTr->position.y, 
            playerRend->texture->getWidth(), playerRend->texture->getHeight() };
        for (uint32_t xp : experiences) {
            TransformComponent xpTr = eManager->getComponentData<TransformComponent>(xp);
            PhysicsComponent xpPhys = eManager->getComponentData<PhysicsComponent>(xp);
            SDL_FRect xpRect = {xpTr.position.x, xpTr.position.y,
                experienceTexture.getWidth(), experienceTexture.getHeight() };
            float dist = getSquaredDistanceBetweenCenters(playerRect, xpRect);
            dist -= stats->collectionRadius * stats->collectionRadius;
            // std::cout << "Calculating for xpID: " << xp << std::endl;
            // std::cout << "Distance and collectionRadius is: " << dist << ", " << stats->collectionRadius << std::endl;
            if (dist <= 0) {
                xpTr.rotDegrees = xpTr.position.angleTowards(playerTr->position) * 180 / PI;
                // std::cout << "Ship Position: " << playerTr->position.x << ", " << playerTr->position.y << std::endl;
                // std::cout << "XP Position: " << xpTr.position.x << ", " << xpTr.position.y << std::endl;
                xpPhys.velocity = 10.0 - (dist) * 0.5;
                // std::cout << "xp rotation and velocity is: " << xpTr.rotDegrees << ", " << xpPhys.velocity << std::endl;
                eManager->setComponentData<PhysicsComponent>(xp, xpPhys);
                eManager->setComponentData<TransformComponent>(xp, xpTr);
            }
        }
    }
}
