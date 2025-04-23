#pragma once

#include "abilities.h"
#include "AbilityMessage.h"
#include "ExplodeMessage.h"
#include "EntityHandle.h"
#include "MessageManager.h"
#include "System.h"
#include "utils.hpp"

#include <tuple>

struct ProjectileConfig {
    EntityType type;
    Texture* texture;
    Shape shape = Shape::Rectangle;
    bool hasLifetime = false;
    bool accelerates = false;
};

class AbilitySystem : public System {
public:
    AbilitySystem(EntityManager* eManager, SDL_Renderer* renderer);
    ~AbilitySystem();
    auto getPlayerComponents(uint32_t eID);
    void update();
    
private:
    void handleAbilityMessage(std::shared_ptr<AbilityMessage> msg);
    void handleExplodeMessage(std::shared_ptr<ExplodeMessage> msg);
    EntityHandle createProjectileEntity();
    void createExplosion(const FPair& pos);
    void spawnProjectile(uint32_t eID, ShipAbilities ability);
    FPair positionProjectile(int index, int total, const FPair& posSource, const float& angleSource,
        const FPair& whSource, ShipAbilities ability);
    FPair positionLinearSpread(int index, int total, const FPair& posSource, const float& angleSource,
        const FPair& whSource, float angleOffset = 0.0f);
    FPair positionRadialSpread(int index, int total, const FPair& posSource, const float& angleSource,
        const FPair& whSource, float angleOffset = 0.0f, float extraRadius = 0.0f);
    void spawnLaserGun(uint32_t eID);
    void spawnRocket(uint32_t eID);
    void spawnGravitySaws(uint32_t eID);
    void spawnLaser(uint32_t eID);
    void spawnExplosives(uint32_t eID);
    EntityManager* eManager;
    std::vector<FPair> rocketsDestroyed;
    Texture explosionTexture, gravitySawTexture, laserTexture, explosiveTexture;
    FPair laserSize;

    std::unordered_map<ShipAbilities, ProjectileConfig> abilityConfigs = {
        {ShipAbilities::LaserGun, {
            .type = EntityType::Shot,
            .texture = &g_shot_texture
        }},
        {ShipAbilities::Rocket, {
            .type = EntityType::Rocket,
            .texture = &g_rocket_texture,
            .hasLifetime = true,
            .accelerates = true
        }},
        {ShipAbilities::Laser, {
            .type = EntityType::Laser,
            .texture = &laserTexture,
            .hasLifetime = true
        }},
        {ShipAbilities::Explosives, {
            .type = EntityType::Explosive,
            .texture = &explosiveTexture,
            .hasLifetime = true
        }},
        {ShipAbilities::GravitySaws, {
            .type = EntityType::GravitySaw,
            .texture = &gravitySawTexture,
            .shape = Shape::Circle,
            .hasLifetime = true
        }}
    };
};
