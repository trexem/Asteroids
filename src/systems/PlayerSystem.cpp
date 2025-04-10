#include "PlayerSystem.h"

PlayerSystem::PlayerSystem(EntityManager* eManager) : eManager(eManager) {
    std::cout << " PlayerSystem Subscribing to: ";
    // Subscribe to KeyboardMessages
    MessageManager::getInstance().subscribe<KeyboardMessage>(
        [this](std::shared_ptr<KeyboardMessage> msg) { handleKeyboardInput(msg); }
    );
    MessageManager::getInstance().subscribe<ExperiencePickupMessage>(
        [this](std::shared_ptr<ExperiencePickupMessage> msg) { handleExperiencePickupMessage(msg); }
    );
}

void PlayerSystem::update(double dT) {
    for (uint32_t eID : eManager->getEntitiesWithComponent(ComponentType::Player)) {
        updateMovement(eID);
        updateAbilities(eID, dT);
    }
}

void PlayerSystem::handleKeyboardInput(std::shared_ptr<KeyboardMessage> msg) {
    auto playerEntities = eManager->getEntitiesWithComponent(ComponentType::Player);

    for (uint32_t eID : playerEntities) {
        MovementComponent moveComp = eManager->getComponentData<MovementComponent>(eID);
        if (msg->key == SDLK_W) moveComp.moveState[MoveState::MOVE_UP] = msg->pressed;
        if (msg->key == SDLK_S) moveComp.moveState[MoveState::MOVE_DOWN] = msg->pressed;
        if (msg->key == SDLK_A) moveComp.moveState[MoveState::MOVE_LEFT] = msg->pressed;
        if (msg->key == SDLK_D) moveComp.moveState[MoveState::MOVE_RIGHT] = msg->pressed;
        eManager->setComponentData<MovementComponent>(eID, moveComp);
    }
}

void PlayerSystem::updateMovement(uint32_t eID) {
    MovementComponent moveComp = eManager->getComponentData<MovementComponent>(eID);
    PhysicsComponent physics = eManager->getComponentData<PhysicsComponent>(eID);
    PlayerComponent playerComp = eManager->getComponentData<PlayerComponent>(eID);
    StatsComponent stats = eManager->getComponentData<StatsComponent>(eID);
    
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
            physics.velocity += stats.speed;
        } else if (moveComp.moveState[MoveState::MOVE_DOWN]) {
            physics.velocity -= stats.speed;
        }
        if (moveComp.moveState[MoveState::MOVE_LEFT]) {
            physics.velocity -= stats.speed;
        } else if (moveComp.moveState[MoveState::MOVE_RIGHT]) {
            physics.velocity += stats.speed;
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
    eManager->setComponentData<PhysicsComponent>(eID, physics);
}

void PlayerSystem::updateAbilities(uint32_t eID, double dT) {
    PlayerComponent player = eManager->getComponentData<PlayerComponent>(eID);
    StatsComponent stats = eManager->getComponentData<StatsComponent>(eID);
    for (size_t i = 0; i < static_cast<size_t>(ShipAbilities::ShipAbilitiesCount); i++) {
        ShipAbilities ability = static_cast<ShipAbilities>(i);

        if (player.abilities.test(i)) {
            int level = player.abilityLevels[static_cast<size_t>(ability)];
            if (level >= 0 && level < 10) {
                double cooldown = abilitiesCooldowns[i][level];
                cooldown *= (1 - stats.fireSpeed);
                player.abilityCooldowns[static_cast<size_t>(ability)] += dT;
                if (player.abilityCooldowns[static_cast<size_t>(ability)] > cooldown) {
                    auto msg = std::make_shared<AbilityMessage>(eID, ability);
                    MessageManager::getInstance().sendMessage(msg);
                    player.abilityCooldowns[static_cast<size_t>(ability)] = 0;
                }
            }
        }
    }
    eManager->setComponentData<PlayerComponent>(eID, player);
}

void PlayerSystem::addAbility(uint32_t eID, ShipAbilities ability) {
    PlayerComponent player = eManager->getComponentData<PlayerComponent>(eID);
    player.abilities[static_cast<size_t>(ability)] = true;
    eManager->setComponentData<PlayerComponent>(eID, player);
}

void PlayerSystem::handleExperiencePickupMessage(std::shared_ptr<ExperiencePickupMessage> msg) {
    PlayerComponent playerComp = eManager->getComponentData<PlayerComponent>(msg->playerID);
    playerComp.currentXp += msg->amount;
    if (playerComp.currentXp >= playerComp.xpToNextLevel) {
        playerComp.level++;
        playerComp.currentXp -= playerComp.xpToNextLevel;
        playerComp.xpToNextLevel = 100 + playerComp.level * 50;
        GameStateManager::getInstance().setState(GameState::LevelUp);
    }
    eManager->setComponentData<PlayerComponent>(msg->playerID, playerComp);
    
}
