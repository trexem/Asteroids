#pragma once

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
