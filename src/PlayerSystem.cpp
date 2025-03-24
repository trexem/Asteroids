#include "PlayerSystem.h"
#include "ApplyForceMessage.h"

PlayerSystem::PlayerSystem(EntityManager* eManager) : eManager(eManager) {
    std::cout << " PlayerSystem Subscribing to: ";
    // Subscribe to KeyboardMessages
    MessageManager::getInstance().subscribe<KeyboardMessage>(
        [this](std::shared_ptr<KeyboardMessage> msg) { handleKeyboardInput(msg); }
    );
}

void PlayerSystem::handleKeyboardInput(std::shared_ptr<KeyboardMessage> msg) {
    auto playerEntities = eManager->getEntitiesWithComponent(ComponentType::Player);

    for (uint32_t eID : playerEntities) {
        FPair velocity;
        if (msg->key == SDLK_W && msg->pressed) velocity.y = -1;
        if (msg->key == SDLK_S && msg->pressed) velocity.y = 1;
        if (msg->key == SDLK_A && msg->pressed) velocity.x = -1;
        if (msg->key == SDLK_D && msg->pressed) velocity.x = 1;
        std::cout << "Sending ApplyForceMessage with eID: " << eID << " and velocity: x: " << velocity.x << " y: " << velocity.y << std::endl;
        auto msg = std::make_shared<ApplyForceMessage>(eID, velocity);
        MessageManager::getInstance().sendMessage(msg);
    }
}
