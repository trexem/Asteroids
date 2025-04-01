#pragma once
#ifndef __ANIMATION_SYSTEM_H
#define __ANIMATION_SYSTEM_H

#include "AnimationMessage.h"
#include "MessageManager.h"
#include "System.h"

class AnimationSystem : public System {
public:
    AnimationSystem(EntityManager* eManager);
    void update(double dT);

private:
    void handleAnimationMessage(std::shared_ptr<AnimationMessage> msg);
    EntityManager* eManager;
};

#endif // !__ANIMATION_SYSTEM_H
