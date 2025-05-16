#pragma once

#include "Camera.h"
#include "DestroyAsteroidMessage.h"
#include "MessageManager.h"
#include "System.h"

class MovementSystem : public System {
public:
    MovementSystem(Camera* camera) : camera(camera) {}
    void update(EntityManager& eMgr, const double& dT) override;
    void updatePlayer(EntityManager& eMgr, const double& dT, uint32_t eID);
    Camera* camera;
};
