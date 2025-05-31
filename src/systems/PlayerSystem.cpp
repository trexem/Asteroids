#include "AbilityMessage.h"
#include "EntityManager.h"
#include "GameSessionManager.h"
#include "GameStateManager.h"
#include "GameStatsManager.h"
#include "KeyboardMessage.h"
#include "LevelUpMessage.h"
#include "MessageManager.h"
#include "PlayerSystem.h"
#include "PickupPickedMessage.h"

PlayerSystem::PlayerSystem(EntityManager& eMgr) : eMgr(eMgr) {
    // Subscribe to KeyboardMessages
    MessageManager::instance().subscribe<KeyboardMessage>(
        [this](std::shared_ptr<KeyboardMessage> msg) { handleKeyboardInput(msg); }
    );
    MessageManager::instance().subscribe<PickupPickedMessage>(
        [this](std::shared_ptr<PickupPickedMessage> msg) { handlePickupPickedMessage(msg); }
    );
    MessageManager::instance().subscribe<LevelUpMessage>(
        [this](std::shared_ptr<LevelUpMessage> msg) { handleLevelUpMessage(msg);  }
    );
}

void PlayerSystem::update(EntityManager& eMgr, const double& dT) {
    if (GameStateManager::instance().getState() == GameState::Playing) {
        for (uint32_t eID : eMgr.getEntitiesWithComponent(ComponentType::Player)) {
            updateMovement(eID);
            updateAbilities(eID, dT);
        }
    }
}

void PlayerSystem::handleKeyboardInput(std::shared_ptr<KeyboardMessage> msg) {
    auto playerEntities = eMgr.getEntitiesWithComponent(ComponentType::Player);

    for (uint32_t eID : playerEntities) {
        MovementComponent moveComp = eMgr.getComponentData<MovementComponent>(eID);
        if (msg->key == SDLK_W) moveComp.moveState[MoveState::MOVE_UP] = msg->pressed;
        if (msg->key == SDLK_S) moveComp.moveState[MoveState::MOVE_DOWN] = msg->pressed;
        if (msg->key == SDLK_A) moveComp.moveState[MoveState::MOVE_LEFT] = msg->pressed;
        if (msg->key == SDLK_D) moveComp.moveState[MoveState::MOVE_RIGHT] = msg->pressed;
        eMgr.setComponentData<MovementComponent>(eID, moveComp);
    }
}

void PlayerSystem::updateMovement(uint32_t eID) {
    MovementComponent moveComp = eMgr.getComponentData<MovementComponent>(eID);
    PhysicsComponent physics = eMgr.getComponentData<PhysicsComponent>(eID);
    PlayerComponent playerComp = eMgr.getComponentData<PlayerComponent>(eID);
    StatsComponent stats = eMgr.getComponentData<StatsComponent>(eID);
    
    if (playerComp.type == ShipType::TANK) {
        if (moveComp.moveState[MoveState::MOVE_UP]) {
            physics.velocity += stats.speed;
        } else if (moveComp.moveState[MoveState::MOVE_DOWN]) {
            physics.velocity -= stats.speed;
        } else if (!moveComp.moveState[MoveState::MOVE_UP] && !moveComp.moveState[MoveState::MOVE_DOWN]) {
            physics.velocity *= 0.95;
            if (std::abs(physics.velocity) < 1) { //if the number is too small we round down to 0
                physics.velocity = 0;
            }
        }
        if (moveComp.moveState[MoveState::MOVE_LEFT]) {
            if (physics.rotVelocity > 0) {
                physics.rotVelocity = 0;
            }
            physics.rotVelocity -= stats.rotationSpeed;
        } else if (moveComp.moveState[MoveState::MOVE_RIGHT]) {
            if (physics.rotVelocity < 0) {
                physics.rotVelocity = 0;
            }
            physics.rotVelocity += stats.rotationSpeed;
        } else if (!moveComp.moveState[MoveState::MOVE_LEFT] && !moveComp.moveState[MoveState::MOVE_RIGHT]) {
            physics.rotVelocity *= 0.95;
            if (std::abs(physics.rotVelocity) < 1) {     //if the number is too small we round down to 0
                physics.rotVelocity = 0;
            }
        }
    } else if (playerComp.type == ShipType::FREE_MOVE) {
        if (moveComp.moveState[MoveState::MOVE_UP]) {
            physics.speed.y -= stats.speed;
        } else if (moveComp.moveState[MoveState::MOVE_DOWN]) {
            physics.speed.y += stats.speed;
        }else if (!moveComp.moveState[MoveState::MOVE_UP] && !moveComp.moveState[MoveState::MOVE_DOWN]) {
            physics.speed.y *= 0.95;
            if (std::abs(physics.speed.y) < 1) { //if the number is too small we round down to 0
                physics.speed.y = 0;
            }
        }
        if (moveComp.moveState[MoveState::MOVE_LEFT]) {
            physics.speed.x -= stats.speed;
        } else if (moveComp.moveState[MoveState::MOVE_RIGHT]) {
            physics.speed.x += stats.speed;
        } else if (!moveComp.moveState[MoveState::MOVE_RIGHT] && !moveComp.moveState[MoveState::MOVE_LEFT]) {
            physics.speed.x *= 0.95;
            if (std::abs(physics.speed.x) < 1) { //if the number is too small we round down to 0
                physics.speed.x = 0;
            }
        }
    }
    if (physics.isSpeedVector) {
        if (physics.speed.x > stats.maxSpeed) { 
            physics.speed.x = stats.maxSpeed; 
        } else if (physics.speed.x < -stats.maxSpeed) { 
            physics.speed.x = -stats.maxSpeed; 
        }
        if (physics.speed.y > stats.maxSpeed) { 
            physics.speed.y = stats.maxSpeed; 
        } else if (physics.speed.y < -stats.maxSpeed) { 
            physics.speed.y = -stats.maxSpeed; 
        }
    }
    if (physics.velocity > stats.maxSpeed) { 
        physics.velocity = stats.maxSpeed; 
    } else if (physics.velocity < stats.minSpeed) {
        physics.velocity = stats.minSpeed;
    }
    if (physics.rotVelocity > stats.maxRotationSpeed) { 
        physics.rotVelocity = stats.maxRotationSpeed; 
    } else if (physics.rotVelocity < -stats.maxRotationSpeed) { 
        physics.rotVelocity = -stats.maxRotationSpeed;
    }
    eMgr.setComponentData<PhysicsComponent>(eID, physics);
}

void PlayerSystem::updateAbilities(uint32_t eID, double dT) {
    PlayerComponent player = eMgr.getComponentData<PlayerComponent>(eID);
    StatsComponent stats = eMgr.getComponentData<StatsComponent>(eID);
    constexpr double burstInterval = 0.25;

    for (size_t i = 0; i < static_cast<size_t>(WeaponAbilities::WeaponAbilitiesCount); i++) {
        WeaponAbilities ability = static_cast<WeaponAbilities>(i);
        int level = player.weaponLevels[i];
        
        if (!player.ownedWeapons.test(i) || level < 0 || level > 10) continue;

        double cooldown = abilitiesCooldowns[i][level] * (1 - stats.fireSpeed);
        if (player.isBursting[i]) {
            player.burstShotTimers[i] += dT;
            if (player.burstShotTimers[i] >= burstInterval) {
                auto msg = std::make_shared<AbilityMessage>(eID, ability);
                MessageManager::instance().sendMessage(msg);
                player.burstShotTimers[i] = 0.0;
                player.shotsRemainingInBurst[i]--;

                if (player.shotsRemainingInBurst[i] <= 0) {
                    player.isBursting[i] = false;
                    player.weaponCooldowns[i] = 0.0;
                }
            }
        } else {
            player.weaponCooldowns[static_cast<size_t>(ability)] += dT;
            if (player.weaponCooldowns[static_cast<size_t>(ability)] > cooldown) {
                if (abilityBursts[i]) {
                    player.isBursting[i] = true;
                    player.burstShotTimers[i] = burstInterval;
                    player.shotsRemainingInBurst[i] = abilitiesProjectileCount[i][level] + stats.projectileCount;
                } else {
                    auto msg = std::make_shared<AbilityMessage>(eID, ability);
                    MessageManager::instance().sendMessage(msg);
                    player.weaponCooldowns[static_cast<size_t>(ability)] = 0;
                }
            }
        }
    }
    eMgr.setComponentData<PlayerComponent>(eID, player);
}

void PlayerSystem::addAbility(uint32_t eID, WeaponAbilities ability) {
    PlayerComponent player = eMgr.getComponentData<PlayerComponent>(eID);
    player.ownedWeapons[static_cast<size_t>(ability)] = true;
    eMgr.setComponentData<PlayerComponent>(eID, player);
}

void PlayerSystem::handlePickupPickedMessage(std::shared_ptr<PickupPickedMessage> msg) {
    if (msg->type & EntityType::Experience) {
        PlayerComponent playerComp = eMgr.getComponentData<PlayerComponent>(msg->playerID);
        playerComp.currentXp += msg->amount;
        if (playerComp.currentXp >= playerComp.xpToNextLevel) {
            playerComp.level++;
            playerComp.currentXp -= playerComp.xpToNextLevel;
            playerComp.xpToNextLevel = 50 + playerComp.level * 50;
            GameStateManager::instance().setState(GameState::LevelUp);
        }
        eMgr.setComponentData<PlayerComponent>(msg->playerID, playerComp);
    } else if (msg->type & EntityType::Gold) {
        GameSessionManager::instance().getStats().gold += msg->amount;
    }
}

void PlayerSystem::handleLevelUpMessage(std::shared_ptr<LevelUpMessage> msg) {
    size_t ability = msg->choice.index;
    uint32_t player = eMgr.getEntitiesWithComponent(ComponentType::Player).at(0);
    PlayerComponent playerComp = eMgr.getComponentData<PlayerComponent>(player);
    if (msg->choice.type == AbilityType::Passive) {
        if (playerComp.ownedPassives[ability]) {
            playerComp.passiveLevels[ability]++;
        } else {
            playerComp.ownedPassives[ability] = true;
            playerComp.ownedPassivesCount++;
        }
        levelUpPassive(player, ability, playerComp.passiveLevels[ability]);
    } else if (msg->choice.type == AbilityType::Weapon) {
        if (playerComp.ownedWeapons[ability]) {
            playerComp.weaponLevels[ability]++;
        }
        else {
            playerComp.ownedWeapons[ability] = true;
            playerComp.ownedWeaponsCount++;
        }
    } else if (msg->choice.type == AbilityType::Money) {
        int gold = 50 * (1 + GameStatsManager::instance().getUpgradeValue(UpgradeType::GoldValue));
        GameStatsManager::instance().addCoins(gold);
        GameStatsManager::instance().save();
    } else if (msg->choice.type == AbilityType::Health) {
        HealthComponent hComp = eMgr.getComponentData<HealthComponent>(player);
        float maxHealth = eMgr.getComponentDataPtr<StatsComponent>(player)->maxHealth;
        hComp.health += 50.0f;
        if (hComp.health > maxHealth) {
            hComp.health = maxHealth;
        }
        eMgr.setComponentData(player, hComp);
    }
    eMgr.setComponentData<PlayerComponent>(player, playerComp);
}

void PlayerSystem::levelUpPassive(uint32_t player, size_t passive, uint8_t level) {
    StatsComponent stats = eMgr.getComponentData<StatsComponent>(player);
    HealthComponent hComp = eMgr.getComponentData<HealthComponent>(player);
    const float value = passiveValues[passive][level];
    switch (passive) {
        case static_cast<size_t>(PassiveAbilities::PickupRadius):
            stats.collectionRadius = value +
                GameStatsManager::instance().getUpgradeValue(UpgradeType::PickupRange);
            break;
        case static_cast<size_t>(PassiveAbilities::CooldownReduction):
            stats.fireSpeed = value + 
                GameStatsManager::instance().getUpgradeValue(UpgradeType::FireRate);
            break;
        case static_cast<size_t>(PassiveAbilities::Armor):
            stats.armor = value +
                GameStatsManager::instance().getUpgradeValue(UpgradeType::Armor);
            break;
        case static_cast<size_t>(PassiveAbilities::MaxHealth): {
            float healthMult = GameStatsManager::instance().getUpgradeValue(UpgradeType::MaxHealth);
            stats.maxHealth = SHIP_BASE_HEALTH * healthMult + value;
            hComp.maxHealth = stats.maxHealth;
            hComp.health += value;
            eMgr.setComponentData(player, hComp);
            break;
        }
        case static_cast<size_t>(PassiveAbilities::HealthRegen):
            stats.healthRegen = value + 
                GameStatsManager::instance().getUpgradeValue(UpgradeType::HealthRegen);
            hComp.regen = stats.healthRegen;
            eMgr.setComponentData(player, hComp);
            break;
        case static_cast<size_t>(PassiveAbilities::MovementSpeed): {
            float speedMult = GameStatsManager::instance().getUpgradeValue(UpgradeType::Speed);
            stats.maxSpeed = static_cast<float>(SHIP_TOP_SPEED * value * speedMult);
            stats.minSpeed = static_cast<float>(SHIP_MIN_SPEED * value * speedMult);
            break;
        }
        case static_cast<size_t>(PassiveAbilities::ProjectileCount):
            stats.projectileCount = value +
                GameStatsManager::instance().getUpgradeValue(UpgradeType::ProjectileCount);
            break;
        case static_cast<size_t>(PassiveAbilities::Size):
            stats.extraSize = value;
            break;
        case static_cast<size_t>(PassiveAbilities::Damage):
            stats.baseDamage = value + 
                GameStatsManager::instance().getUpgradeValue(UpgradeType::Damage);
            break;
        case static_cast<size_t>(PassiveAbilities::ProjectileSpeed):
            stats.projectileSpeed = value;
            break;
        default:
            break;
    }
    eMgr.setComponentData<StatsComponent>(player, stats);
}
