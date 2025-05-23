#include "AnimationSystem.h"

#include <iostream>

AnimationSystem::AnimationSystem(EntityManager& eManager) : eMgr(eManager) {
    MessageManager::instance().subscribe<AnimationMessage>(
        [this](std::shared_ptr<AnimationMessage> msg) { handleAnimationMessage(msg); }
    );
}

void AnimationSystem::handleAnimationMessage(std::shared_ptr<AnimationMessage> msg) {
    auto anim = eMgr.getComponentData<AnimationComponent>(msg->eID);
    if (anim.animations.count(msg->animation)) {
        anim.current = msg->animation;
        anim.playing = true;
        anim.currentFrame = 0;
        anim.timeAccumulator = 0;
        eMgr.setComponentData(msg->eID, anim);
    }
}

void AnimationSystem::update(EntityManager& eMgr, const double& dT) {
    for (uint32_t eID : eMgr.getEntitiesWithComponent(ComponentType::Animation)) {
        auto anim = eMgr.getComponentData<AnimationComponent>(eID);
        auto render = eMgr.getComponentData<RenderComponent>(eID);

        if (!anim.playing || anim.animations.empty()) continue;

        auto& state = anim.animations[anim.current];
        anim.timeAccumulator += dT;

        if (anim.timeAccumulator >= state.frameDuration) {
            anim.timeAccumulator -= state.frameDuration;
            anim.currentFrame++;

            if (anim.currentFrame >= static_cast<int>(state.frames.size())) {
                if (state.looping) {
                    anim.currentFrame = 0;
                } else {
                    anim.playing = false;
                    anim.currentFrame = static_cast<int>(state.frames.size()) - 1;
                }
            }
        }

        render.texture = state.frames[anim.currentFrame];
        render.visibility = anim.visible ? 255 : 0;

        eMgr.setComponentData(eID, anim);
        eMgr.setComponentData(eID, render);
    }
}
