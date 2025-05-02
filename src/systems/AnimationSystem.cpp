#include "AnimationSystem.h"

#include <iostream>

AnimationSystem::AnimationSystem(EntityManager* eManager) : eManager(eManager) {
    MessageManager::instance().subscribe<AnimationMessage>(
        [this](std::shared_ptr<AnimationMessage> msg) { handleAnimationMessage(msg); }
    );
}

void AnimationSystem::handleAnimationMessage(std::shared_ptr<AnimationMessage> msg) {
    AnimationComponent anim = eManager->getComponentData<AnimationComponent>(msg->eID);
    anim.playingAnimation[static_cast<size_t>(msg->animation)] = true;
    eManager->setComponentData<AnimationComponent>(msg->eID, anim);
}

void AnimationSystem::update(double dT) {
    for (uint32_t eID : eManager->getEntitiesWithComponent(ComponentType::Animation)) {
        AnimationComponent anim = eManager->getComponentData<AnimationComponent>(eID);
        RenderComponent texture = eManager->getComponentData<RenderComponent>(eID);
        if (anim.playingAnimation[static_cast<size_t>(Animation::Damage)]) {
            // std::cout << "Animating eID: " << eID;
            anim.frameTime += dT;
            anim.elapsedTime += dT;

            if (anim.frameTime >= BLINK_INTERVAL) {
                anim.visible = !anim.visible;
                anim.frameTime = 0;
            }
            if (anim.elapsedTime >= BLINK_DURATION) {
                anim.playingAnimation[static_cast<size_t>(Animation::Damage)] = false;
                anim.elapsedTime = 0;
                anim.frameTime = 0;
                anim.visible = true;
            }
        }

        if (anim.visible) {
            texture.texture->setAlphaMod(255);
        } else {
            texture.texture->setAlphaMod(0);
        }
        eManager->setComponentData<AnimationComponent>(eID, anim);
    }
}
