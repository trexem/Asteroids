#pragma once

#include "weaponAbilities.h"
#include "AbilityMessage.h"
#include "ExplodeMessage.h"
#include "EntityHandle.h"
#include "MessageManager.h"
#include "TextureManager.h"
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

struct ExplosionConfig {
    FPair pos;
    WeaponAbilities source;
};


class AbilitySystem : public System {
public:
    static constexpr double EXPLOSION_LIFETIME = 0.5;

    AbilitySystem(EntityManager& eManager, SDL_Renderer* renderer);
    ~AbilitySystem();
    auto getPlayerComponents(uint32_t eID);
    void update(EntityManager& eMgr, const double& dT) override;
    
private:
    void handleAbilityMessage(std::shared_ptr<AbilityMessage> msg);
    void handleExplodeMessage(std::shared_ptr<ExplodeMessage> msg);
    EntityHandle createProjectileEntity();
    void createExplosion(const ExplosionConfig& pos);
    void spawnProjectile(uint32_t eID, WeaponAbilities ability);
    FPair positionProjectile(int index, int total, const FPair& posSource, const float& angleSource,
        const FPair& whSource, WeaponAbilities ability);
    FPair positionLinearSpread(int index, int total, const FPair& posSource, const float& angleSource,
        const FPair& whSource, float angleOffset = 0.0f);
    FPair positionRadialSpread(int index, int total, const FPair& posSource, const float& angleSource,
        const FPair& whSource, float angleOffset = 0.0f, float extraRadius = 0.0f);
    void spawnLaserGun(uint32_t eID);
    void spawnRocket(uint32_t eID);
    void spawnGravitySaws(uint32_t eID);
    void spawnLaser(uint32_t eID);
    void spawnExplosives(uint32_t eID);
    EntityManager& eMgr;
    std::vector<ExplosionConfig> explosions;
    FPair laserSize;

    std::unordered_map<WeaponAbilities, ProjectileConfig> abilityConfigs = {
        {WeaponAbilities::LaserGun, {
            .type = EntityType::Shot,
            .texture = TextureManager::instance().get("shot")
        }},
        {WeaponAbilities::Rocket, {
            .type = EntityType::Rocket,
            .texture = TextureManager::instance().get("rocket"),
            .hasLifetime = true,
            .accelerates = true
        }},
        {WeaponAbilities::Laser, {
            .type = EntityType::Laser,
            .texture = TextureManager::instance().get("laserBeam"),
            .hasLifetime = true
        }},
        {WeaponAbilities::Explosives, {
            .type = EntityType::Explosive,
            .texture = TextureManager::instance().get("explosive"),
            .hasLifetime = true
        }},
        {WeaponAbilities::GravitySaws, {
            .type = EntityType::GravitySaw,
            .texture = TextureManager::instance().get("gravitySaw"),
            .shape = Shape::Circle,
            .hasLifetime = true
        }}
    };
};
