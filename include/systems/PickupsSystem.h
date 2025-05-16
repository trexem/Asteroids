#pragma once

#include "EntityType.h"
#include "GameStatsManager.h"
#include "MessageManager.h"
#include "PickupsSpawnMessage.h"
#include "System.h"
#include "texture.hpp"
#include "utils.hpp"

struct PickupConfig {
    Texture* texture;
};

class PickupsSystem : public System {
public:
    PickupsSystem(EntityManager& eMgr, SDL_Renderer* renderer);
    void update(EntityManager& eMgr, const double& dT) override;

private:
    void handlePickupsSpawnMessage(std::shared_ptr<PickupsSpawnMessage> msg);
    EntityManager& eMgr;
    Texture experienceTexture;
    Texture goldTexture;

    std::unordered_map<EntityType, PickupConfig> pickupConfigs = {
        {EntityType::Experience, {
            .texture = &experienceTexture,
        }},
        {EntityType::Gold, {
            .texture = &goldTexture
        }},
    };
};
