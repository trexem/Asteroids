#include "AnimationSystem.h"

#include <iostream>

AnimationSystem::AnimationSystem(EntityManager& eManager) : eMgr(eManager) {
    MessageManager::instance().subscribe<AnimationMessage>(
        [this](std::shared_ptr<AnimationMessage> msg) { handleAnimationMessage(msg); }
    );
}

void AnimationSystem::handleAnimationMessage(std::shared_ptr<AnimationMessage> msg) {
    AnimationComponent anim = eMgr.getComponentData<AnimationComponent>(msg->eID);
    anim.playingAnimation[static_cast<size_t>(msg->animation)] = true;
    eMgr.setComponentData<AnimationComponent>(msg->eID, anim);
}

void AnimationSystem::update(EntityManager& eMgr, const double& dT) {
    for (uint32_t eID : eMgr.getEntitiesWithComponent(ComponentType::Animation)) {
        AnimationComponent anim = eMgr.getComponentData<AnimationComponent>(eID);
        RenderComponent texture = eMgr.getComponentData<RenderComponent>(eID);
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
        eMgr.setComponentData<AnimationComponent>(eID, anim);
    }
}
