#include "PlayerSystem.h"

PlayerSystem::PlayerSystem(EntityManager* eManager) : eManager(eManager) {
    std::cout << " PlayerSystem Subscribing to: ";
    // Subscribe to KeyboardMessages
    MessageManager::getInstance().subscribe<KeyboardMessage>(
        [this](std::shared_ptr<KeyboardMessage> msg) { handleKeyboardInput(msg); }
    );
}

void PlayerSystem::update() {
    for (uint32_t eID : eManager->getEntitiesWithComponent(ComponentType::Player)) {
        MovementComponent moveComp = eManager->getComponentData<MovementComponent>(eID);
        PhysicsComponent physics = eManager->getComponentData<PhysicsComponent>(eID);
        PlayerComponent playerComp = eManager->getComponentData<PlayerComponent>(eID);
        StatsComponent statsComp = eManager->getComponentData<StatsComponent>(eID);
        if (playerComp.shipType) {
            if (moveComp.moveState[MoveState::MOVE_UP]) {
                physics.velocity.y += statsComp.speed;
            } else if (moveComp.moveState[MoveState::MOVE_DOWN]) {
                physics.velocity.y -= statsComp.speed;
            } else if (!moveComp.moveState[MoveState::MOVE_UP] && !moveComp.moveState[MoveState::MOVE_DOWN]) {
                physics.velocity.y *= 0.95;
                if (std::abs(physics.velocity.y) < 1) { //if the number is too small we round down to 0
                    physics.velocity.y = 0;
                }
            }
            if (moveComp.moveState[MoveState::MOVE_LEFT]) {
                if (physics.rotVelocity > 0) {
                    physics.rotVelocity = 0;
                }
                physics.rotVelocity -= statsComp.rotationSpeed;
            } else if (moveComp.moveState[MoveState::MOVE_RIGHT]) {
                if (physics.rotVelocity < 0) {
                    physics.rotVelocity = 0;
                }
                physics.rotVelocity += statsComp.rotationSpeed;
            } else if (!moveComp.moveState[MoveState::MOVE_LEFT] && !moveComp.moveState[MoveState::MOVE_RIGHT]) {
                physics.rotVelocity *= 0.95;
                if (std::abs(physics.rotVelocity) < 1) {     //if the number is too small we round down to 0
                    physics.rotVelocity = 0;
                }
            }
        } else {
            if (moveComp.moveState[MoveState::MOVE_UP]) {
                physics.velocity.y += statsComp.speed;
            } else if (moveComp.moveState[MoveState::MOVE_DOWN]) {
                physics.velocity.y -= statsComp.speed;
            }
            if (moveComp.moveState[MoveState::MOVE_LEFT]) {
                physics.velocity.x -= statsComp.speed;
            } else if (moveComp.moveState[MoveState::MOVE_RIGHT]) {
                physics.velocity.x += statsComp.speed;
            }
        }
        if (physics.velocity.y > statsComp.maxSpeed) { 
            physics.velocity.y = statsComp.maxSpeed; 
        } else if (physics.velocity.y < statsComp.minSpeed) {
            physics.velocity.y = statsComp.minSpeed;
        }
        if (physics.velocity.x > statsComp.maxSpeed) { 
            physics.velocity.x = statsComp.maxSpeed; 
        } else if (physics.velocity.x < statsComp.minSpeed) {
            physics.velocity.x = statsComp.minSpeed; 
        }
        if (physics.rotVelocity > statsComp.maxRotationSpeed) { 
            physics.rotVelocity = statsComp.maxRotationSpeed; 
        } else if (physics.rotVelocity < -statsComp.maxRotationSpeed) { 
            physics.rotVelocity = -statsComp.maxRotationSpeed;
        }
        eManager->setComponentData<PhysicsComponent>(eID, physics);
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
